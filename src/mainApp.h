#pragma once

#include "RealEngine/baseApp.h"
#include <RealEngine/stop_watch.h>

namespace re{

class MainApp
{
    static StopWatch previous_time;
public:
    static long delta_time;
    static void init();
    static void tick();

    static IBaseAppPtr baseApp;
};

}  // namespace re