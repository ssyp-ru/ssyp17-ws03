#pragma once

#include "math.h"

namespace re
{

class Camera
{
public:
    void view_at( re::Point2f pos );
    void scale( float coff );
    re::Point2f world_to_screen( re::Point2f pos );
    re::Point2f screen_to_world( re::Point2f pos );
private:
    re::Point2f cam_pos;
    float coff;
};

}