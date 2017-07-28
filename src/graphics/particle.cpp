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
            OpenGL::instance().draw_image_part( curr.pos.x, curr.pos.y, 
                                                curr.pos.x+curr.size.x, curr.pos.y+curr.size.y, 
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

void explodeImage( Point2f pos, Point2f size, Point2f part, ImagePtr imge, Point2f center, Point2f impulse, float power, float randF )
{
    stParticle nPart;
    nPart.imge = imge;
    nPart.life = 50;
    nPart.move = Point2f(0,0);
    nPart.vec = Point2f(0,0);

    for( int i = 0; i < part.x; i++ )
    {
        for( int j = 0; j < part.y; j++ )
        {
            float xa = ( (float)i / (float)part.x );
            float ya = ( (float)j / (float)part.y );

            float xs = ( 1.0f / (float)part.x );
            float ys = ( 1.0f / (float)part.y );

            nPart.pos = Point2f( pos.x + (size.x * xa),
                                 pos.y + (size.y * ya) );

            nPart.size = Point2f( size.x / part.x,
                                  size.y / part.y );

            nPart.startImg = Point2f( xa, ya );
            nPart.endImg = Point2f( xa + xs, ya + ys );

            float xr = ((float)(rand() % 100)) / 100.0f;
            xr*= randF;
            xr = 1 - xr;
            float yt = ((float)(rand() % 100)) / 100.0f;
            yt*= randF;
            yt = 1 - yt;

            //nPart.vec = Point2f( i+0.5, j+0.5 );
            nPart.move = Point2f(   (((xa + xs*0.5) - center.x) * impulse.x) * xr,
                                    impulse.y * yt );
                                    
            nPart.vec = Point2f( ((xa + xs*0.5) - center.x) * power,
                                 ((ya + ys*0.5) - center.y) * power );
            
            launchParticle( nPart );
        }
    }
}

}