#include "weakPlatform.h"

re::ImagePtr WeakPlatform::tex = nullptr;

WeakPlatform::WeakPlatform(re::Vector2f pos, re::Vector2f siz, double destroyTime) : Platform::Platform(pos, siz)
{
    size = siz;
    touched = false;
    hp = destroyTime;
    setFriction(1.0);
    setBounciness(0.0);
}

void WeakPlatform::onCollisionStay(re::GameObjectPtr to, re::Vector2f vec)
{
    if (re::Vector2f(0, 1).angleBetween(vec) < 30.0 / 180.0 * 3.14159)
        touched = true;
}

void WeakPlatform::update()
{
    if (touched) hp -= re::Time::delta_time;
    if (hp <= 0)
    {
        destroy();
        re::explode_image(  re::Point2f( position.X * 16, position.Y * 16 ), 
                            re::Point2f( 64, 64 ), 
                            re::Point2f( 6, 6 ), 
                            tex, 
                            re::Point2f( 0.5, -6 ),
                            re::Point2f( 5, -4 ),
                            1,
                            1 );
    }
}

void WeakPlatform::display(int scale)
{
    if( tex == nullptr )
    {
        tex = std::make_shared<re::Image>( "map/corr.png" );
    }

    re::draw_image_part( position.X * scale, position.Y * scale, (position.X * scale) + 64, (position.Y * scale) + 64,
                            0,0, 1,1,
                            tex );
}