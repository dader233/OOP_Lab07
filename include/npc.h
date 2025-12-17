#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>
#include <mutex>
#include <shared_mutex>

class Observer;

class Npc {
protected:
    unsigned int x;
    unsigned int y;
    std::string unique_name;
    bool alive;
    unsigned int distance_step;
    unsigned int distance_kill;
    std::vector<std::shared_ptr<Observer>> observers;

public:
    Npc() = default;
    Npc(const unsigned int x, const unsigned int y, const std::string &name);
    Npc(Npc &&other) noexcept;
    virtual ~Npc() = default;

    virtual std::string info() const = 0;
    virtual bool accept(std::shared_ptr<Npc> defender) const = 0;

    unsigned int get_x() const;
    unsigned int get_y() const;
    std::string get_name() const;
    bool is_alive() const;
    void make_dead();
    
    unsigned int get_distance_step() const;
    unsigned int get_distance_kill() const;
    
    void move(int shift_x, int shift_y, unsigned int max_value);

    void attach(std::shared_ptr<Observer> observer);
    void detach(std::shared_ptr<Observer> observer);
    void notify(Npc &defender);

    virtual std::ofstream &save(std::ofstream &os) const;
    virtual std::ostream &print(std::ostream &os) const;

    friend std::istream &operator>>(std::istream &is, Npc &npc);
};

std::ostream &print_all_npc(std::ostream &os, const std::set<std::shared_ptr<Npc>> &set_npc);