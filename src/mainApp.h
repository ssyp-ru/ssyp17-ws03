#pragma once

#include "RealEngine/baseApp.h"

namespace re{

class MainApp
{
    static StopWatch timer_;
    public:
    static long delta_time;
    static void init();
    static void tick();

    static IBaseAppPtr baseApp;
};

}  // namespace re