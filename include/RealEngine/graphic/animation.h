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
    Animation(float speed, bool cycleable);
    Animation(); //goes 1 by default

    void add_frame(ImagePtr img);
    ImagePtr getNextFrame();
    void setSpeed(float source);
    float getSpeed();
};
typedef std::shared_ptr<Animation> AnimationPtr;


} //namespace re