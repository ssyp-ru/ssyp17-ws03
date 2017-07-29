#pragma once
#include <RealEngine/stop_watch.h>

namespace re
{

class Time
{
private:
    static StopWatch previous_time;
public:
    static double delta_time;
    static void updateTime()
    {
        StopWatch now;
        delta_time = now.diff(previous_time) / 1e9;
        previous_time = now;

    }    
};

} // namespace re