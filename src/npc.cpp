#include "../include/npc.h"
#include "../include/observer.h"
#include <memory>
#include <stdexcept>

std::istream &operator>>(std::istream &is, Npc &npc) {
    is >> npc.x >> npc.y >> npc.unique_name;
    return is;
}

Npc::Npc(Npc &&other) noexcept {
    unique_name = std::move(other.unique_name);
    x = other.x;
    y = other.y;
    alive = other.alive;
    distance_step = other.distance_step;
    distance_kill = other.distance_kill;
    observers = std::move(other.observers);
}

Npc::Npc(const unsigned int x, const unsigned int y, const std::string &name) {
    // Проверка координат
    if (x > 500 || y > 500) {
        throw std::invalid_argument("Coordinates must be between 0 and 500");
    }
    
    static std::set<std::string> names;
    static unsigned int count = 0;

    this->x = x;
    this->y = y;
    this->alive = true;
    this->distance_step = 0;  // устанавливается в производных классах
    this->distance_kill = 0;  // устанавливается в производных классах
    
    std::string copy_name = name;
    if (names.count(name) != 0) {
        copy_name += "_" + std::to_string(count++);
    }
    this->unique_name = std::move(copy_name);
    names.insert(this->unique_name);
}

unsigned int Npc::get_x() const {
    return this->x;
}

unsigned int Npc::get_y() const {
    return this->y;
}

std::string Npc::get_name() const {
    return this->unique_name;
}

bool Npc::is_alive() const {
    return this->alive;
}

void Npc::make_dead() {
    this->alive = false;
}

unsigned int Npc::get_distance_step() const {
    return this->distance_step;
}

unsigned int Npc::get_distance_kill() const {
    return this->distance_kill;
}

void Npc::move(int shift_x, int shift_y, unsigned int max_value) {
    // Проверяем, не выходим ли за границы по X
    int new_x = static_cast<int>(this->x) + shift_x;
    if (new_x >= 0 && new_x <= static_cast<int>(max_value)) {
        this->x = static_cast<unsigned int>(new_x);
    }
    
    // Проверяем, не выходим ли за границы по Y
    int new_y = static_cast<int>(this->y) + shift_y;
    if (new_y >= 0 && new_y <= static_cast<int>(max_value)) {
        this->y = static_cast<unsigned int>(new_y);
    }
}

void Npc::attach(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
}

void Npc::detach(std::shared_ptr<Observer> observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Npc::notify(Npc &defender) {
    for (auto &observer : observers) {
        if (observer) {
            observer->update(*this, defender);
        }
    }
}

std::ofstream &Npc::save(std::ofstream &os) const {
    os << this->info();
    return os;
}

std::ostream &Npc::print(std::ostream &os) const {
    os << this->info();
    return os;
}

std::ostream &print_all_npc(std::ostream &os, const std::set<std::shared_ptr<Npc>> &set_npc) {
    for (auto &npc : set_npc) {
        npc->print(os) << '\n';
    }
    return os;
}