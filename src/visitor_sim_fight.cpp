#include "../include/visitor_sim_fight.h"
#include <algorithm>

bool validate_fight(const std::shared_ptr<Npc> &npc_1, const std::shared_ptr<Npc> &npc_2, 
                    const int distance) {
    if (npc_1 == npc_2) {
        return false;
    }

    int max_x = static_cast<int>(std::max(npc_1->get_x(), npc_2->get_x()));
    int min_x = static_cast<int>(std::min(npc_1->get_x(), npc_2->get_x()));
    int max_y = static_cast<int>(std::max(npc_1->get_y(), npc_2->get_y()));
    int min_y = static_cast<int>(std::min(npc_1->get_y(), npc_2->get_y()));

    return (max_x - min_x <= distance && max_y - min_y <= distance);
}

std::set<std::shared_ptr<Npc>> simulate_fight(const std::set<std::shared_ptr<Npc>> &set_npc, 
                                              const int distance) {
    std::set<std::shared_ptr<Npc>> dead_list;

    for (auto &attacker : set_npc) {
        for (auto &defender : set_npc) {
            if (!validate_fight(attacker, defender, distance)) continue;

            if (attacker->accept(defender)) {
                dead_list.insert(defender);
                attacker->notify(*defender);
            }
        }
    }
    return dead_list;
}

bool VisitorFightOrc::fight(std::shared_ptr<Npc> defender) {
    if (std::dynamic_pointer_cast<Druid>(defender)) {
        return true;
    }
    return false;
}

bool VisitorFightSquirrel::fight(std::shared_ptr<Npc> defender) {
    return false;
}

bool VisitorFightDruid::fight(std::shared_ptr<Npc> defender) {
    if (std::dynamic_pointer_cast<Squirrel>(defender)) {
        return true;
    }
    return false;
}