#pragma once
#include "stop_watch.h"
#include <fstream>
#include <string>

namespace re {
    class Log {
    public:
        enum class LEVEL {
             NONE, INFO, DEBUG, TRACE
        }; // Log::LEVEL::INFO
        static void set_console_level(LEVEL);       
        static void set_file_level(LEVEL);       
        static void set_screen_level(LEVEL);       

        static re::Log::LEVEL get_console_level();       
        static re::Log::LEVEL get_file_level();       
        static re::Log::LEVEL get_screen_level();       

        // returns time since execution of program
        static long time();        

        // for syntax Log.stream() << ... << endl;
        // endl required for flush
        static std::ofstream& stream();
        
        // prints message to log, in format "> N: [StopWatch.getTimeString : time()] msg"
        // returns index of msg printed.
        static uint file_msg(std::string, LEVEL);
        static uint console_msg(std::string, LEVEL);
        static uint screen_msg();
        static void add_screen_msg(std::string, LEVEL);
        static uint msg(std::string, LEVEL);

        static uint trace (std::string);    
        static uint debug (std::string);
        static uint info (std::string);

        // same as stream, but also writes default overhead of msg
        // +"> N: [StopWatch.getTimeString : time()]"
        // endl or flush required
        static std::ofstream& stream_msg();
        // overloaded if N is needed.
        static std::ofstream& stream_msg(int&);
    };
}