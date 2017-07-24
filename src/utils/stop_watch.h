#pragma once
#include <chrono>
#include <string>

namespace re {
    class StopWatch {
        std::chrono::high_resolution_clock::time_point start_time;
    public:
        // initializes 
        StopWatch();

        // returns time elapsed in nanoseconds since 'this' construction
        long stop_watch();
        // returns time between given StopWatch and 'this' construction
        long diff(const StopWatch& time_point);
        // do /1000 or /1000000 for micro- and milliseconds if needed
        
        // returns current time in format of current locale
        static std::string getTimeString();
    };
}