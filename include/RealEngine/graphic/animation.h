#pragma once
#include "RealEngine/graphic.h"
#include "RealEngine/graphic/image.h"
#include <vector>
#include "stb_image.h"

namespace re{

class Animation{
    private:
    std::vector<ImagePtr> images;
    bool isCyclic;
    float animationSpeed;
    float curPosition = 0;

    public:
    void add_frame(ImagePtr img);
    ImagePtr getNextFrame();
    Animation(float speed, bool cycleable);
    Animation(); //goes 1 by default
    void setSpeed(float source);
    float getSpeed();
};
} //namespace re