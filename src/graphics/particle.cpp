#include "particle.h"

#include "../window/opengl.h"

#include <iostream>
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
// #include <GL/glx.h>

using std::vector;

namespace re
{

const unsigned int particleLimit = 1000;
const unsigned int particleSystemLimit = 10;

std::vector<stParticle> allParticles;
std::vector<stParticleSystem> allSystems;
//ImagePtr gloImage;

int findLastParticle()
{
    /*for( int i = lastParticle; i < particleLimit; i++ )
    {
        if( allParticles[i].life <= 0 )
        {
            lastParticle = i;
            return;
        }
    }*/

    for( unsigned int i = 0; i < particleLimit; i++ )
    {
        if( allParticles[i].life <= 0 )
        {
            return i;
        }
    }

    return -1;
}

int findLastParticleSystem()
{
    for( unsigned int i = 0; i < particleSystemLimit; i++ )
    {
        if( allSystems[i].life <= 0 )
        {
            return i;
        }
    }

    return -1;
}

void launchParticleSystem( stParticleSystem &system )
{
    int lastParticleSystem = findLastParticleSystem();
    if( lastParticleSystem == -1 )
    {
        std::cout << "too many particle systems";
        return;
    }
    allSystems.push_back( system );
}

void launch_particle( stParticle &newParticle )
{
    int lastParticle = findLastParticle();
    if( lastParticle == -1 )
    {
        std::cerr << "too many particle\n";
        return;
    }
    allParticles[lastParticle] = newParticle;
}

void drawParticleSystem( float delta )
{
    for( auto &sys : allSystems )
    {
        if( sys.life > 0 )
        {
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D,sys.imge->getTex());

            glBegin(GL_QUADS);

            for( stParticle &curr : sys.particles )
            {
                glTexCoord2f(curr.startImg.x,curr.startImg.y); glVertex3f( curr.pos.x, -curr.pos.y, 10 );
                glTexCoord2f(curr.startImg.x,curr.endImg.y); glVertex3f( curr.pos.x, -curr.pos.y-curr.size.y, 10 );
                glTexCoord2f(curr.endImg.x,curr.endImg.y); glVertex3f( curr.pos.x+curr.size.x, -curr.pos.y-curr.size.y, 10 );
                glTexCoord2f(curr.endImg.x,curr.startImg.y); glVertex3f( curr.pos.x+curr.size.x, -curr.pos.y, 10 );

                curr.move.x += curr.vec.x * delta;
                curr.move.y += curr.vec.y * delta;
                
                curr.pos.x += curr.move.x * delta;
                curr.pos.y += curr.move.y * delta;

            }

            glEnd();
            glDisable(GL_TEXTURE_2D);

            sys.life -= delta;
        }
    }
}

void drawParticles( float delta )
{
    drawParticleSystem( delta );
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

    for( unsigned int i = 0; i < particleLimit; i++ )
    {
        allParticles.push_back( nPatricle );
    }

    stParticleSystem systems;
    systems.life = 0;
    
    for( unsigned int i = 0; i < particleSystemLimit; i++ )
    {
        allSystems.push_back( systems );
    }
}

void explode_image( Point2f pos, Point2f size, Point2f part, ImagePtr imge, Point2f center, Point2f impulse, float power, float randF )
{
    stParticle nPart;
    //nPart.imge = imge;
    //nPart.life = 50;
    nPart.move = Point2f(0,0);
    nPart.vec = Point2f(0,0);

    stParticleSystem nSys;
    nSys.life = 50;
    nSys.imge = imge;

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
            
            //launchParticle( nPart );
            nSys.particles.push_back( nPart );
        }
    }

    launchParticleSystem( nSys );
}

}