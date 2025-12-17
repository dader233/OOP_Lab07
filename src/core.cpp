#include "../include/core.h"
#include "../include/visitor_sim_fight.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

FightFunctor::FightFunctor(std::shared_ptr<bool> is_work_thread,
        std::shared_ptr<std::shared_mutex> mtx,
        std::shared_ptr<std::mutex> mtx_cout) :
    is_work_thread(is_work_thread), mtx(mtx), mtx_cout(mtx_cout) {
    events = std::make_shared<std::queue<FightEvent>>();
}

FightFunctor::FightFunctor(const FightFunctor &other) {
    events = other.events;
    is_work_thread = other.is_work_thread;
    mtx = other.mtx;
    mtx_cout = other.mtx_cout;
}

void FightFunctor::add_event(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender) {
    events->emplace(attacker, defender);
}

bool FightFunctor::throw_the_dice() {
    unsigned int power_attack = (std::rand() % 6) + 1;
    unsigned int power_defender = (std::rand() % 6) + 1;
    return power_attack > power_defender;
}

void FightFunctor::operator()() {
    while (*this->is_work_thread) {
        while (!events->empty()) {
            FightEvent current_event = events->front();
            events->pop();

            std::lock_guard<std::shared_mutex> lock(*mtx);
            std::shared_ptr<Npc>& attacker = current_event.attacker;
            std::shared_ptr<Npc>& defender = current_event.defender;
            
            if (attacker->is_alive() && defender->is_alive()) {
                if (attacker->accept(defender)) {
                    if (throw_the_dice()) {
                        defender->make_dead();
                        std::lock_guard<std::mutex> lock_cout(*mtx_cout);
                        attacker->notify(*defender);
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

MoveFunctor::MoveFunctor(std::set<std::shared_ptr<Npc>> &set_npc,
                         std::shared_ptr<FightFunctor> fight_functor_ptr,
                         const unsigned int MAX_VALUE,
                         std::shared_ptr<bool> is_work_thread,
                         std::shared_ptr<std::shared_mutex> mtx,
                         std::shared_ptr<std::mutex> mtx_cout) :
    set_npc(set_npc), fight_functor_ptr(fight_functor_ptr), 
    MAX_VALUE(MAX_VALUE), is_work_thread(is_work_thread), 
    mtx(mtx), mtx_cout(mtx_cout) {}

void MoveFunctor::operator()() {
    while (*this->is_work_thread) {
        std::lock_guard<std::shared_mutex> lock(*mtx);
        
        // Удаляем мертвых NPC
        for (auto it = set_npc.begin(); it != set_npc.end(); ) {
            if (!(*it)->is_alive()) {
                it = set_npc.erase(it);
            } else {
                ++it;
            }
        }
        
        // Перемещаем живых NPC и проверяем бои
        for (auto &attacker : set_npc) {
            if (!attacker->is_alive()) continue;
            
            // Случайное перемещение в пределах distance_step
            int shift_x = (std::rand() % (2 * attacker->get_distance_step() + 1)) - attacker->get_distance_step();
            int shift_y = (std::rand() % (2 * attacker->get_distance_step() + 1)) - attacker->get_distance_step();
            
            attacker->move(shift_x, shift_y, MAX_VALUE);
            
            // Проверяем возможность боя с другими NPC
            for (auto &defender : set_npc) {
                if (attacker == defender || !defender->is_alive()) continue;
                
                if (is_murder_available(attacker, defender)) {
                    fight_functor_ptr->add_event(attacker, defender);
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

bool is_murder_available(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender) {
    if (attacker == defender) {
        return false;
    }

    const unsigned int max_x = std::max(attacker->get_x(), defender->get_x());
    const unsigned int min_x = std::min(attacker->get_x(), defender->get_x());
    const unsigned int max_y = std::max(attacker->get_y(), defender->get_y());
    const unsigned int min_y = std::min(attacker->get_y(), defender->get_y());

    const unsigned int distance_in_a_square = (max_x - min_x) * (max_x - min_x) + 
                                            (max_y - min_y) * (max_y - min_y);
    
    const unsigned int kill_distance = attacker->get_distance_kill();
    const unsigned int kill_distance_sq = kill_distance * kill_distance;

    return distance_in_a_square <= kill_distance_sq;
}

std::set<std::shared_ptr<Npc>> generate_npc(const unsigned int MAX_VALUE) {
    std::set<std::shared_ptr<Npc>> set_npc;
    
    auto stdin_observer = std::make_shared<StdinObserver>();
    auto log_file_observer = std::make_shared<FileObserver>("log.txt");
    
    const int N = 50;
    for (int i = 0; i < N; i++) {
        const int rand_type = std::rand() % 3;
        TypeNpc type;
        std::string base_name;
        
        switch (rand_type) {
            case 0:
                type = TypeNpc::orc;
                base_name = "Orc";
                break;
            case 1:
                type = TypeNpc::squirrel;
                base_name = "Squirrel";
                break;
            case 2:
                type = TypeNpc::druid;
                base_name = "Druid";
                break;
        }
        
        unsigned int x = std::rand() % MAX_VALUE;
        unsigned int y = std::rand() % MAX_VALUE;
        
        std::shared_ptr<Npc> npc = FactoryNpc::create_npc(type, base_name, x, y);
        npc->attach(stdin_observer);
        npc->attach(log_file_observer);
        set_npc.insert(npc);
    }
    
    return set_npc;
}

void print_alive_npc(const std::set<std::shared_ptr<Npc>> &set_npc) {
    std::cout << "\n=== Surviving NPCs after 30 seconds ===\n";
    int count = 0;
    for (auto &npc : set_npc) {
        if (npc->is_alive()) {
            npc->print(std::cout) << '\n';
            count++;
        }
    }
    std::cout << "Total survivors: " << count << "\n";
}

void start_program() {
    const unsigned int MAX_VALUE = 100;
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    std::set<std::shared_ptr<Npc>> set_npc = generate_npc(MAX_VALUE);
    
    std::shared_ptr<bool> is_work_thread = std::make_shared<bool>(true);
    std::shared_ptr<std::shared_mutex> mtx_ptr = std::make_shared<std::shared_mutex>();
    std::shared_ptr<std::mutex> mtx_cout_ptr = std::make_shared<std::mutex>();
    
    FightFunctor fight_functor(is_work_thread, mtx_ptr, mtx_cout_ptr);
    std::shared_ptr<FightFunctor> fight_functor_ptr = std::make_shared<FightFunctor>(fight_functor);
    MoveFunctor move_functor(set_npc, fight_functor_ptr, MAX_VALUE, is_work_thread, mtx_ptr, mtx_cout_ptr);
    
    std::thread fight_thread(std::ref(fight_functor));
    std::thread move_thread(std::ref(move_functor));
    
    auto begin = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
    
    while (elapsed_time.count() < 30) {
        std::vector<std::vector<char>> grid(MAX_VALUE + 1, std::vector<char>(MAX_VALUE + 1, '.'));
        
        {
            std::shared_lock<std::shared_mutex> lock(*mtx_ptr);
            for (auto &npc : set_npc) {
                if (!npc->is_alive()) continue;
                unsigned int x = npc->get_x();
                unsigned int y = npc->get_y();
                
                if (x < grid.size() && y < grid[x].size()) {
                    char symbol = ' ';
                    if (dynamic_cast<Orc*>(npc.get())) symbol = 'O';
                    else if (dynamic_cast<Squirrel*>(npc.get())) symbol = 'S';
                    else if (dynamic_cast<Druid*>(npc.get())) symbol = 'D';
                    
                    if (symbol != ' ') {
                        grid[x][y] = symbol;
                    }
                }
            }
        }
        
        {
            std::lock_guard<std::mutex> lock(*mtx_cout_ptr);
            std::cout << "\n=== Map (Time: " << elapsed_time.count() << "s) ===\n";
            for (size_t i = 0; i < grid.size(); i++) {
                for (size_t j = 0; j < grid[i].size(); j++) {
                    std::cout << grid[i][j];
                }
                std::cout << '\n';
            }
            std::cout << "Legend: O=Orc, S=Squirrel, D=Druid, .=empty\n";
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        end = std::chrono::steady_clock::now();
        elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
    }
    
    *is_work_thread = false;
    
    move_thread.join();
    fight_thread.join();
    
    print_alive_npc(set_npc);
}