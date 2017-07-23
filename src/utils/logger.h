#include <fstream>
#include <string>
#include <chrono>

namespace re {
    class Log {
        std::ofstream stream_;

        // initializes upon construction or can be set manually
        std::chrono::high_resolution_clock::time_point start_time;
        // initializes upon .track() call
        std::chrono::high_resolution_clock::time_point track_time;

    public:
        ~Log(); // closes the stream automatically

        // Opens a file with given name and writes log there. 
        Log(const char* output);
        Log(const char* output, std::chrono::high_resolution_clock::time_point);
        
        // returns time since construction in milliseconds
        // (or custom time_point given given to constructor)
        long time();
        
        
        // for syntax <Log>.stream() << ... << endl;
        // endl required for flush
        std::ofstream& stream();
        
        // returns time in format of current locale
        std::string getTimeString();
        
        // prints message to log, in format "> [getTimeString : time()] msg"
        void msg(const char* msg);
        void msg(std::string msg);

        // starts track
        void track();
        
        // returns time elapsed since last .track() in nanoseconds
        long track_elapsed();
    };
}