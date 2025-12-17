#include "../include/observer.h"

FileObserver::FileObserver(const std::string &filename) {
    log_file.open(filename, std::ios::app);
    if (!log_file.is_open()) {
        throw std::logic_error("Could not open log file: " + filename);
    }
}

FileObserver::~FileObserver() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

void FileObserver::update(const Npc &attacker, const Npc &defender) {
    if (log_file.is_open()) {
        log_file << "[file observer]: ";
        attacker.save(log_file);
        log_file << " killed ";
        defender.save(log_file);
        log_file << '\n';
        log_file.flush();
    }
}

void StdinObserver::update(const Npc &attacker, const Npc &defender) {
    std::cout << "[stdin observer]: ";
    attacker.print(std::cout);
    std::cout << " killed ";
    defender.print(std::cout);
    std::cout << '\n';
}