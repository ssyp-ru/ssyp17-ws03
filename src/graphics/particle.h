#include <RealEngine/math.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic.h>

#include <vector>

namespace re
{

struct stParticleSystem
{
    std::vector<stParticle> particles;
    ImagePtr imge;
    float life;
};

void drawParticles( float delta );
void initParticles();

}