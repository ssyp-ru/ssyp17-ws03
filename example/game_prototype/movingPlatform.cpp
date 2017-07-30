#include "movingPlatform.h"

re::ImagePtr MovingPlatform::tex = nullptr;

MovingPlatform::MovingPlatform(re::Vector2f pos, re::Vector2f size, double speed) : Platform::Platform(pos, size) 
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

    re::draw_image_part( position.X * scale, position.Y * scale, (position.X * scale) + 64, (position.Y * scale) + 16,
                            0,0.25, 1,0.75,
                            tex );
}
