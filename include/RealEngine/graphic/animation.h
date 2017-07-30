#pragma once
#include "RealEngine/graphic.h"
#include "RealEngine/graphic/image.h"
#include <vector>
#include "stb_image.h"

namespace re{

class Animation{
private:
    std::vector<ImagePtr> images; //All frames of animation
    bool isCyclic;
    float animationSpeed;

public:
    float curPosition = 0;
    Animation(float speed, bool cycleable);
    Animation();

    void add_frame(ImagePtr img);  //Adds new frame to animation
    ImagePtr getNextFrame();    //Changes curPosition and returns frame
    ImagePtr getFrame(size_t index); //Returns frame at "index" position
    void setCurrentPos(float pos);
    void setSpeed(float source);
    float getSpeed();
};
typedef std::shared_ptr<Animation> AnimationPtr;


} //namespace re