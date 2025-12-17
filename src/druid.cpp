#include "../include/druid.h"
#include "../include/visitor_sim_fight.h"

Druid::Druid(unsigned int x, unsigned int y, const std::string &name) 
    : Npc(x, y, name) {
    this->distance_step = 10;  // по таблице для Друида
    this->distance_kill = 10;  // по таблице для Друида
}

Druid::Druid(Druid &&other) noexcept : Npc(std::move(other)) {}

std::string Druid::info() const {
    return "druid(" + std::to_string(this->x) + ' ' + std::to_string(this->y) + ") " + 
           '[' + this->unique_name + ']';
}

bool Druid::accept(std::shared_ptr<Npc> defender) const {
    VisitorFightDruid visitor;
    return visitor.fight(defender);
}