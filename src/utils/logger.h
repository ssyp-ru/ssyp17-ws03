#pragma once
#include "stop_watch.h"
#include <fstream>
#include <string>

namespace re {
    class Log {
        std::ofstream stream_;
        StopWatch clock_;

        // each msg has its index which is returned by
        unsigned int msgCount;

    public:
        ~Log(); // closes the stream automatically

        // Opens a file with given name and writes log there.
        Log(const char* outputFile);
        Log(const char* outputFile, StopWatch clock);
        
        // returns time since construction in milliseconds
        // (or custom StopWatch given given to constructor)
        long time();        
        
        // for syntax <Log>.stream() << ... << endl;
        // endl required for flush
        std::ofstream& stream();
        
        // prints message to log, in format "> N: [timer.getTimeString : time()] msg"
        // returns index of msg printed. 
        unsigned int msg(const char* msg);
        unsigned int msg(std::string msg);
    };
}