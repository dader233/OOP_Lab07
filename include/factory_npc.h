#pragma once

#include <memory>
#include <fstream>
#include <string>
#include "npc.h"
#include "orc.h"
#include "squirrel.h"
#include "druid.h"

enum class TypeNpc {
    orc,
    squirrel,
    druid
};

class FactoryNpc {
public:
    static std::shared_ptr<Npc> create_npc(const TypeNpc &type, const std::string &name, 
                                           unsigned int x, unsigned int y);
    
    static std::shared_ptr<Npc> create_npc_from_file(std::ifstream &in);
};