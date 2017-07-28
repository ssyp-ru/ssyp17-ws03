#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "player.h"

void Player::updatePosition(){
    playercoords.x += curVelocity;
}
void Player::move(bool isForward){
    movingAnim.setSpeed(0.7);
    if (isForward) curVelocity = moveVelocity; // TODO: physics improvement
    else curVelocity = -moveVelocity;
    //draw_image(playercoords.x, playercoords.y, movingAnim.getNextFrame());
}
void Player::stop(){ // stops player
    curVelocity = 0;
    movingAnim.setSpeed(0);
    //draw_image(playercoords.x, playercoords.y, movingAnim.getNextFrame());
}
Player::Player(int startingX, int startingY, int vel, const re::Animation& movAnim)
    : playercoords(startingX, startingY){
        moveVelocity = vel;
        movingAnim = movAnim;
}

re::Point2f Player::getCurPosition(){
    return re::Point2f(playercoords.x, playercoords.y);
} // returns current position

void Player::display(){
    re::draw_image(playercoords.x, playercoords.y, movingAnim.getNextFrame());
}
