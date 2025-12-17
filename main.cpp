#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>

#include "include/core.h"
#include "include/factory_npc.h"
#include "include/observer.h"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    std::cout << "=== Lab 07: Multi-threaded NPC Simulation ===\n";
    std::cout << "Variant 1: Orc, Squirrel, Druid\n";
    std::cout << "Movement distances: Orc=20, Squirrel=5, Druid=10\n";
    std::cout << "Kill distances: Orc=10, Squirrel=5, Druid=10\n";
    std::cout << "Map: 100x100\n";
    std::cout << "NPCs: 50 randomly placed\n";
    std::cout << "Duration: 30 seconds\n";
    std::cout << "Rules: Orc kills Druid, Druid kills Squirrel, Squirrel is peaceful\n";
    std::cout << "Combat: 6-sided dice determines attack/defense\n";
    std::cout << "Starting simulation...\n\n";
    
    start_program();
    
    return 0;
}