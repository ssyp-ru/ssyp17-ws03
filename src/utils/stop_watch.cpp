#include "stop_watch.h"
#include <ctime>

std::string re::StopWatch::getTimeString() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *now_tm = std::localtime(&now_c);

    char buff[64];
    size_t size = strftime(buff, sizeof(buff), "%x %X", now_tm);
    return std::string(buff);
}

re::StopWatch::StopWatch() {
    start_time = std::chrono::high_resolution_clock::now();
}

long re::StopWatch::stop_watch() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time).count();
}
long re::StopWatch::diff(const StopWatch& time_point) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(start_time - time_point.start_time).count();
}