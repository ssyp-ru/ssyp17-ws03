#include "RealEngine/camera.h"

using namespace re;

void Camera::view_at( re::Point2f pos ) {
    this->cam_pos_ = pos;
}

void Camera::translate( re::Point2f offset ) {
    this->cam_pos_ += offset;
}

re::Point2f Camera::get_cam_pos() {
    return this->cam_pos_;
}

void Camera::scale( float coff ) {
    this->coff_ = coff;
}

re::Point2f Camera::world_to_screen( re::Point2f pos ) {
    pos -= this->cam_pos_;
    pos /= this->coff_;
    return pos;
}

re::Point2f Camera::screen_to_world( re::Point2f pos ) {
    //world 0,0
    //camer 10, 10
    //coffe 2
    // in   10,10
    pos *= this->coff_;
    pos += this->cam_pos_;
    return pos;
}

float Camera::meter_to_screen( float unit )
{
    return unit / this->coff_;
}

float Camera::screen_to_meter( float unit )
{
    return unit * this->coff_;
}