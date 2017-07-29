#pragma once

#include "RealEngine/baseApp.h"

namespace re{

class MainApp
{
public:
    static void init();
    static void tick();

    static IBaseAppPtr baseApp;
};

}  // namespace re