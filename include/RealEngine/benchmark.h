#pragma once

#include <chrono>
#include <iostream>

class TimeCounter {
public:
    TimeCounter(const char* prefix) 
        : prefix_(prefix)
    {
        start = std::chrono::steady_clock::now();
    }

    ~TimeCounter(){
        long long mc = std::chrono::duration_cast<std::chrono::microseconds>
            (std::chrono::steady_clock::now() - start).count();
        std::cout << prefix_ << " " << mc / 1000.0 << "ms\n";
    }

private:
    const char* prefix_;
    std::chrono::steady_clock::time_point start;
};