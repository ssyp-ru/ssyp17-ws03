#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "player.h"
#include <iostream>

/*void Player::move(bool isForward){
    //movingAnim.setSpeed(0.7);
}*/
void Player::display(int scale){
    re::draw_image(position.X * scale, position.Y * scale, movingAnim.getNextFrame());
}
