#pragma once

#include <set>
#include <memory>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "npc.h"
#include "factory_npc.h"
#include "observer.h"

struct FightEvent {
    std::shared_ptr<Npc> attacker;
    std::shared_ptr<Npc> defender;
    
    FightEvent(std::shared_ptr<Npc> a, std::shared_ptr<Npc> d) 
        : attacker(a), defender(d) {}
};

class FightFunctor {
private:
    std::shared_ptr<std::queue<FightEvent>> events;
    std::shared_ptr<bool> is_work_thread;
    std::shared_ptr<std::shared_mutex> mtx;
    std::shared_ptr<std::mutex> mtx_cout;
    
public:
    FightFunctor(std::shared_ptr<bool> is_work_thread,
                 std::shared_ptr<std::shared_mutex> mtx,
                 std::shared_ptr<std::mutex> mtx_cout);
    
    FightFunctor(const FightFunctor &other);
    
    void add_event(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender);
    
    bool throw_the_dice();
    
    void operator()();
};

class MoveFunctor {
private:
    std::set<std::shared_ptr<Npc>> &set_npc;
    std::shared_ptr<FightFunctor> fight_functor_ptr;
    const unsigned int MAX_VALUE;
    std::shared_ptr<bool> is_work_thread;
    std::shared_ptr<std::shared_mutex> mtx;
    std::shared_ptr<std::mutex> mtx_cout;
    
public:
    MoveFunctor(std::set<std::shared_ptr<Npc>> &set_npc,
                std::shared_ptr<FightFunctor> fight_functor_ptr,
                const unsigned int MAX_VALUE,
                std::shared_ptr<bool> is_work_thread,
                std::shared_ptr<std::shared_mutex> mtx,
                std::shared_ptr<std::mutex> mtx_cout);
    
    void operator()();
};

bool is_murder_available(std::shared_ptr<Npc> attacker, std::shared_ptr<Npc> defender);

std::set<std::shared_ptr<Npc>> generate_npc(const unsigned int MAX_VALUE);

void print_alive_npc(const std::set<std::shared_ptr<Npc>> &set_npc);

void start_program();