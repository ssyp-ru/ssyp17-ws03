#include "particle.h"

#include "../window/opengl.h"

#include <iostream>
#include <vector>

using std::vector;

namespace re
{

std::vector<stParticle> allParticles;
int lastParticle = 0;

void findLastParticle()
{
    for( int i = lastParticle; i < 500; i++ )
    {
        if( allParticles[i].life <= 0 )
        {
            lastParticle = i;
            return;
        }
    }

    for( int i = 0; i < lastParticle; i++ )
    {
        if( allParticles[i].life <= 0 )
        {
            lastParticle = i;
            return;
        }
    }

    lastParticle = -1;
}

void launchParticle( stParticle &newParticle )
{
    findLastParticle();
    if( lastParticle == -1 )
    {
        std::cerr << "too many particle\n";
        return;
    }
    allParticles[lastParticle] = newParticle;
}

void drawParticles( float delta )
{
    for( stParticle &curr : allParticles )
    {
        if( curr.life > 0 )
        {
            std::cout << curr.life << std::endl;
            OpenGL::instance().draw_image_part( curr.pos.x, curr.pos.y, 
                                                curr.pos.x+5, curr.pos.y+5, 
                                                curr.startImg.x, curr.startImg.y, 
                                                curr.endImg.x, curr.endImg.y, 
                                                curr.imge );
            
            curr.move.x += curr.vec.x * delta;
            curr.move.y += curr.vec.y * delta;
            
            curr.pos.x += curr.move.x * delta;
            curr.pos.y += curr.move.y * delta;

            curr.life -= delta;
        }
    }
}

void initParticles()
{
    stParticle nPatricle;
    nPatricle.life = 0;
    nPatricle.imge = nullptr;
    nPatricle.endImg = Point2f(0,0);
    nPatricle.startImg = Point2f(0,0);

    for( int i = 0; i < 500; i++ )
    {
        allParticles.push_back( nPatricle );
    }
}

}