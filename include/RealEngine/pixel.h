#pragma once

#include <stdlib.h>
#include <stdint.h>

namespace re
{

const float to_screen_coff = 2;
const size_t map_size_w = 1000 / to_screen_coff;
const size_t map_size_h = 600 / to_screen_coff;

class PixelObject
{
public:
    int x, y;
    float ang;
    float vec_x, vec_y;
    float vec_ang;


};

class PixelEngine
{
public:

    struct pixel
    {
        unsigned char color[3];
    };

    PixelEngine();
    void draw();
    void update( float delta );

    void spam_static_pixels( int x, int y );
    void spam_dynamic_pixels( int x, int y, float life );
    void spam_powder( int x, int y );
    
    void add_static_pixel( int x, int y, int r, int g, int b);

    bool castRay( int &x0, int &y0, int &x1, int &y1 );
    void get_normal( float &x, float &y );
public:

    unsigned char map_color[map_size_h][map_size_w][4];

    unsigned short int dynamic_pos[map_size_h * map_size_w][2];
    float dynamic_vec[map_size_h * map_size_w][2];
    float dynamic_vec_to[map_size_h * map_size_w][2];
    float dynamic_life_time[map_size_h * map_size_w];
    size_t dynamic_last_free;
    size_t dynamic_count;
    bool dynamic_alive[map_size_h * map_size_w];

    unsigned int glid;

    bool map_dynamic[map_size_h][map_size_w];
    unsigned int map_dynamic_ptr[map_size_h][map_size_w];
    bool map_pixel[map_size_h][map_size_w];
};

} // namespace re