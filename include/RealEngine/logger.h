#pragma once
#include "stop_watch.h"
#include <fstream>
#include <string>

namespace re {
    class Log {
    public:
        enum class LEVEL {
            INFO, DEBUG, TRACE
        }; // Log::LEVEL::INFO
        static void log_level(LEVEL);       

        // returns time since execution of program
        static long time();        

        // for syntax Log.stream() << ... << endl;
        // endl required for flush
        static std::ofstream& stream();
        
        // prints message to log, in format "> N: [StopWatch.getTimeString : time()] msg"
        // returns index of msg printed.
        static unsigned int msg(std::string, LEVEL);
        
        // same as stream, but also writes default overhead of msg
        // +"> N: [StopWatch.getTimeString : time()]"
        // endl or flush required
        static std::ofstream& stream_msg();
        // overloaded if N is needed.
        static std::ofstream& stream_msg(int&);
    };
}