#include "movingPlatform.h"

re::ImagePtr MovingPlatform::tex = nullptr;

MovingPlatform::MovingPlatform(re::Point2f pos, re::Point2f size, double speed) : Platform::Platform(pos, size) 
{
    path = new WaypointPath(speed, (DrawableGameObject*)this);
}

void MovingPlatform::update()
{
    path->update();
}

void MovingPlatform::display(int scale)
{
    if( tex == nullptr )
    {
        tex = std::make_shared<re::Image>( "map/metal.png" );
    }

    re::draw_image_part( position.x * scale, position.y * scale, (position.x * scale) + 64, (position.y * scale) + 16,
                            0,0.25, 1,0.75,
                            tex );
}
