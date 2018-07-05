#pragma once

#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/physic_core.h>
#include <RealEngine/graphic/image.h>
#include <iostream>
#include <vector>
#include <memory>

namespace re {
class BasicPlatform : public re::GameObject{
    private:
    re::ImagePtr texture

    public:
    //quadrangle platform, rectangles recommended
    BasicPlatform(Vector2f center, Vector2f point1, Vector2f point2, Vector2f point3,
    Vector2f point4, re::ImagePtr plTexture) : re::GameObject::GameObject(center) {
        addPoint(point1);
        addPoint(point2);
        addPoint(point3);
        addPoint(point4);
        addEdge(0, 1);
        addEdge(1, 2);
        addEdge(2, 3);
        addEdge(3, 0);
        texture = plTexture;
    } 

};
}