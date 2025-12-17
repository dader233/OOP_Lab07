#include "../include/orc.h"
#include "../include/visitor_sim_fight.h"

Orc::Orc(unsigned int x, unsigned int y, const std::string &name) 
    : Npc(x, y, name) {
    this->distance_step = 20;  // по таблице для Орка
    this->distance_kill = 10;  // по таблице для Орка
}

Orc::Orc(Orc &&other) noexcept : Npc(std::move(other)) {}

std::string Orc::info() const {
    return "orc(" + std::to_string(this->x) + ' ' + std::to_string(this->y) + ") " + 
           '[' + this->unique_name + ']';
}

bool Orc::accept(std::shared_ptr<Npc> defender) const {
    VisitorFightOrc visitor;
    return visitor.fight(defender);
}