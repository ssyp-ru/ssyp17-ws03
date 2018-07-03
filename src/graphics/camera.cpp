#include "RealEngine/camera.h"

using namespace re;

void Camera::view_at( re::Point2f pos ) {
    this->cam_pos = pos;
}

void Camera::scale( float coff ) {
    this->coff = coff;
}

re::Point2f Camera::world_to_screen( re::Point2f pos ) {
    pos -= this->cam_pos;
    pos /= this->coff;
    return pos;
}

re::Point2f Camera::screen_to_world( re::Point2f pos ) {
    //world 0,0
    //camer 10, 10
    //coffe 2
    // in   10,10
    pos *= this->coff;
    pos += this->cam_pos;
    return pos;
}

float Camera::metr_to_screen( float unit )
{
    return unit / this->coff;
}

float Camera::screen_to_metr( float unit )
{
    return unit * this->coff;
}