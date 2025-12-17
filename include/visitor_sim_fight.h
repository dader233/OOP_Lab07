#pragma once

#include <memory>
#include <set>
#include "npc.h"
#include "orc.h"
#include "squirrel.h"
#include "druid.h"

bool validate_fight(const std::shared_ptr<Npc> &npc_1, const std::shared_ptr<Npc> &npc_2, 
                    const int distance);
std::set<std::shared_ptr<Npc>> simulate_fight(const std::set<std::shared_ptr<Npc>> &set_npc, 
                                              const int distance);

class VisitorFightOrc {
public:
    bool fight(std::shared_ptr<Npc> defender);
};

class VisitorFightSquirrel {
public:
    bool fight(std::shared_ptr<Npc> defender);
};

class VisitorFightDruid {
public:
    bool fight(std::shared_ptr<Npc> defender);
};