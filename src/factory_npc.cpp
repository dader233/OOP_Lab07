#include "../include/factory_npc.h"
#include <stdexcept>
#include <cctype>
#include <algorithm>

std::string to_lower(const std::string &str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::shared_ptr<Npc> FactoryNpc::create_npc(const TypeNpc &type, const std::string &name, 
                                           unsigned int x, unsigned int y) {
    switch (type) {
        case TypeNpc::orc:
            return std::make_shared<Orc>(x, y, name);
        case TypeNpc::squirrel:
            return std::make_shared<Squirrel>(x, y, name);
        case TypeNpc::druid:
            return std::make_shared<Druid>(x, y, name);
        default:
            throw std::invalid_argument("Unknown NPC type");
    }
}

std::shared_ptr<Npc> FactoryNpc::create_npc_from_file(std::ifstream &in) {
    if (!in.is_open()) {
        throw std::logic_error("File is not open");
    }
    
    std::string type_str;
    in >> type_str;
    
    if (in.eof()) {
        throw std::runtime_error("End of file reached");
    }
    
    unsigned int x, y;
    std::string name;
    
    in >> x >> y >> name;
    
    if (in.fail()) {
        throw std::runtime_error("Failed to read NPC data from file");
    }
    
    type_str = to_lower(type_str);
    TypeNpc type;
    
    if (type_str == "orc") {
        type = TypeNpc::orc;
    } else if (type_str == "squirrel") {
        type = TypeNpc::squirrel;
    } else if (type_str == "druid") {
        type = TypeNpc::druid;
    } else {
        throw std::invalid_argument("Unknown NPC type in file: '" + type_str + "'");
    }
    
    return create_npc(type, name, x, y);
}