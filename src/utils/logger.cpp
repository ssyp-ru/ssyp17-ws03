#include "logger.h"
#include <ctime>

long re::Log::time() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
}

std::string re::Log::getTimeString() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *now_tm = std::localtime(&now_c);

    char buff[64];
    size_t size = strftime(buff, sizeof(buff), "%x %X", now_tm);
    return std::string(buff);
}

void re::Log::msg(const char* msg) {
    stream_ << "> [" << getTimeString() << " : " << time() << "] " << msg << std::endl;
}
void re::Log::msg(std::string msg) {
    stream_ << "> [" << getTimeString() << " : " << time() << "] " << msg << std::endl;
}

re::Log::~Log() {
    msg("Closing the log.");
    stream_.close();
}
re::Log::Log(const char* output) {
    start_time = std::chrono::high_resolution_clock::now();
    stream_ = std::ofstream(output);
    msg("Log opened successfully.");
}
re::Log::Log(const char* output, std::chrono::high_resolution_clock::time_point start_time_) {
    start_time = start_time_;
    stream_ = std::ofstream(output);
    msg("Log opened successfully with custom time.");
}

std::ofstream& re::Log::stream() { return stream_; }

void re::Log::track() {
    track_time = std::chrono::system_clock::now();
}
long re::Log::track_elapsed() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now - track_time).count();
}