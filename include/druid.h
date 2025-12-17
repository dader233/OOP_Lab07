#pragma once

#include "npc.h"

class Druid : public Npc {
public:
    Druid() = default;
    Druid(unsigned int x, unsigned int y, const std::string &name);
    Druid(Druid &&other) noexcept;
    
    std::string info() const override;
    bool accept(std::shared_ptr<Npc> defender) const override;
};