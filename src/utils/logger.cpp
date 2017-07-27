#include "logger.h"

long re::Log::time() {
    return clock_.stop_watch()/1000000;
}

unsigned int re::Log::msg(std::string msg) {
    stream_ << "> " << ++msgCount << ": [" << clock_.getTimeString() << " : " << time() << "] " << msg << std::endl;
    return msgCount;
}

re::Log::~Log() {
    msg("Closing the log.");
    stream_.close();
}
re::Log::Log(std::string outputFile) {
    msgCount = 0;
    clock_ = StopWatch();
    stream_ = std::ofstream(outputFile);
    msg("Log opened successfully.");
}
re::Log::Log(std::string outputFile, StopWatch clock) {
    msgCount = 0;
    clock_ = clock;
    stream_ = std::ofstream(outputFile);
    msg("Log opened successfully with custom clock.");
}

std::ofstream& re::Log::stream() { return stream_; }