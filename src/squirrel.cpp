#include "../include/squirrel.h"
#include "../include/visitor_sim_fight.h"

Squirrel::Squirrel(unsigned int x, unsigned int y, const std::string &name) 
    : Npc(x, y, name) {
    this->distance_step = 5;   // по таблице для Белки
    this->distance_kill = 5;   // по таблице для Белки
}

Squirrel::Squirrel(Squirrel &&other) noexcept : Npc(std::move(other)) {}

std::string Squirrel::info() const {
    return "squirrel(" + std::to_string(this->x) + ' ' + std::to_string(this->y) + ") " + 
           '[' + this->unique_name + ']';
}

bool Squirrel::accept(std::shared_ptr<Npc> defender) const {
    VisitorFightSquirrel visitor;
    return visitor.fight(defender);
}