#pragma once

#include "npc.h"

class Squirrel : public Npc {
public:
    Squirrel() = default;
    Squirrel(unsigned int x, unsigned int y, const std::string &name);
    Squirrel(Squirrel &&other) noexcept;
    
    std::string info() const override;
    bool accept(std::shared_ptr<Npc> defender) const override;
};