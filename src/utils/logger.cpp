#include "logger.h"

re::Log::Log() {
    l1_ = std::ofstream("l1.txt");
}

std::ofstream& re::Log::l1() { return l1_; }