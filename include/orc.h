#pragma once

#include "npc.h"

class Orc : public Npc {
public:
    Orc() = default;
    Orc(unsigned int x, unsigned int y, const std::string &name);
    Orc(Orc &&other) noexcept;
    
    std::string info() const override;
    bool accept(std::shared_ptr<Npc> defender) const override;
};