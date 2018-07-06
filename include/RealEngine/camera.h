#pragma once

#include "math.h"

namespace re
{

class Camera
{
public:
    void view_at( re::Point2f pos );
    void translate( re::Point2f offset );
    re::Point2f get_cam_pos();

    void scale( float coff );
    re::Point2f world_to_screen( re::Point2f pos );
    re::Point2f screen_to_world( re::Point2f pos );
    float meter_to_screen( float unit );
    float screen_to_meter( float unit );
private:
    re::Point2f cam_pos_ = {0,0};
    float coff_ = 0;
};

}