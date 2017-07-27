#include "RealEngine/graphic.h"
#include "RealEngine/graphic/image.h"
#include "RealEngine/graphic/animation.h"
#include "stb_image.h"
#include <vector>
#include <iostream>

namespace re{

    void Animation::add_frame(ImagePtr img){ //fills images 
        images.push_back(img);
    }
    ImagePtr Animation::getNextFrame(){ //count which img to draw
        if (images.size() == 0) throw "no elements in animation found";
        curPosition += animationSpeed;
        if (curPosition >= images.size()) curPosition = 0;
        return images[(int) curPosition];
    }
    Animation::Animation(float speed, bool cycleable){
        if (speed < 0) throw "animation speed lesser than 0";
        animationSpeed = speed;
        isCyclic = cycleable;
    }
    Animation::Animation(){
        animationSpeed = 1;
        isCyclic = true;
    }
    void Animation::setSpeed(float source){
        if (source < 0) throw "animation speed lesser than 0";
        animationSpeed = source;
    }
    float Animation::getSpeed(){
        return animationSpeed;
    }

} //namespace re