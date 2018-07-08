#include <RealEngine/pixel.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>

#include <math.h>

namespace re
{

PixelEngine::PixelEngine()
{
    for( size_t i = 0; i < map_size_h; i++ )
    {
        for( size_t j = 0; j < map_size_w; j++ )
        {
            this->map_pixel[i][j] = false;
            this->map_color[i][j][0] = 255;
            this->map_color[i][j][1] = 255;
            this->map_color[i][j][2] = 255;
            this->map_color[i][j][3] = 0;
        }
    }

    for( size_t i = 0; i < map_size_h; i++ )
    {
        this->map_pixel[i][0] = true;
        this->map_color[i][0][0] = 127;
        this->map_color[i][0][1] = 127;
        this->map_color[i][0][2] = 127;
        this->map_color[i][0][3] = 255;

        this->map_pixel[i][1] = true;
        this->map_color[i][1][0] = 127;
        this->map_color[i][1][1] = 127;
        this->map_color[i][1][2] = 127;
        this->map_color[i][1][3] = 255;

        this->map_pixel[i][map_size_w-1] = true;
        this->map_color[i][map_size_w-1][0] = 127;
        this->map_color[i][map_size_w-1][1] = 127;
        this->map_color[i][map_size_w-1][2] = 127;
        this->map_color[i][map_size_w-1][3] = 255;

        this->map_pixel[i][map_size_w-2] = true;
        this->map_color[i][map_size_w-2][0] = 127;
        this->map_color[i][map_size_w-2][1] = 127;
        this->map_color[i][map_size_w-2][2] = 127;
        this->map_color[i][map_size_w-2][3] = 255;
    }

    for( size_t j = 0; j < map_size_w; j++ )
    {
        this->map_pixel[0][j] = true;
        this->map_color[0][j][0] = 127;
        this->map_color[0][j][1] = 127;
        this->map_color[0][j][2] = 127;
        this->map_color[0][j][3] = 255;

        this->map_pixel[1][j] = true;
        this->map_color[1][j][0] = 127;
        this->map_color[1][j][1] = 127;
        this->map_color[1][j][2] = 127;
        this->map_color[1][j][3] = 255;

        this->map_pixel[map_size_h-1][j] = true;
        this->map_color[map_size_h-1][j][0] = 127;
        this->map_color[map_size_h-1][j][1] = 127;
        this->map_color[map_size_h-1][j][2] = 127;
        this->map_color[map_size_h-1][j][3] = 255;

        this->map_pixel[map_size_h-2][j] = true;
        this->map_color[map_size_h-2][j][0] = 127;
        this->map_color[map_size_h-2][j][1] = 127;
        this->map_color[map_size_h-2][j][2] = 127;
        this->map_color[map_size_h-2][j][3] = 255;
    }

    dynamic_count = 0;
    dynamic_last_free = 0;

    glGenTextures( 1, &glid );
}

void PixelEngine::spam_static_pixels( int x, int y )
{
    for( int i = x; i < x+50; i++ )
    {
        for( int j = y; j < y+50; j++ )
        {
            this->map_pixel[i][j] = true;
            this->map_dynamic[i][j] = false;
            this->map_color[i][j][0] = 35;
            this->map_color[i][j][1] = 35;
            this->map_color[i][j][2] = 35;
            this->map_color[i][j][3] = 255;
        }
    }
}

bool PixelEngine::castRay( int &x0, int &y0, int &x1, int &y1 )
{
    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    int x = x0;
    int y = y0;
    int xinc1, xinc2, yinc1, yinc2;

    if (x1 >= x0) { // The x-values are increasing     
        xinc1 = 1;
        xinc2 = 1;
    }
    else { // The x-values are decreasing
        xinc1 = -1;
        xinc2 = -1;
    }
    if (y1 >= y0) { // The y-values are increasing
        yinc1 = 1;
        yinc2 = 1;
    }
    else { // The y-values are decreasing
        yinc1 = -1;
        yinc2 = -1;
    }

    int den, num, numadd, numpixels;
    if (deltax >= deltay) { // There is at least one x-value for every y-value
        xinc1 = 0; // Don't change the x when numerator >= denominator
        yinc2 = 0; // Don't change the y for every iteration
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax; // There are more x-values than y-values
    }
    else { // There is at least one y-value for every x-value
        xinc2 = 0; // Don't change the x for every iteration
        yinc1 = 0; // Don't change the y when numerator >= denominator
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay; // There are more y-values than x-values
    }
    int prevX = x0;
    int prevY = y0;

    for (int curpixel = 0; curpixel <= numpixels; curpixel++) {
        if (this->map_pixel[x][y] == true)
        {
            x0 = prevX;
            y0 = prevY;
            x1 = x;
            y1 = y;
            return true;
        }
            
        prevX = x;
        prevY = y;
        
        num += numadd; // Increase the numerator by the top of the fraction
        
        if (num >= den) {  // Check if numerator >= denominator
            num -= den; // Calculate the new numerator value
            x += xinc1; // Change the x as appropriate
            y += yinc1; // Change the y as appropriate
        }
        
        x += xinc2; // Change the x as appropriate
        y += yinc2; // Change the y as appropriate
    }  

    x0 = prevX;
    y0 = prevY;
    x1 = x;
    y1 = y;
    return false; // nothing was found
}

void PixelEngine::get_normal( float &x, float &y )
{
    int nx = x;
    int ny = y;
    x = 0;
    y = 0;
    for( int i = -3; i <= 3; i++ )
    {
        for(int j = -3; j <= 3; j++ )
        {
            if ( this->map_pixel[nx+i][ny+j] && !this->map_dynamic[nx+i][ny+i] ) {
                x -= i;
                y -= j;
            }
        }
    }
    float len = sqrtf((x*x) + (y*y));
    x /= len;
    y /= len;
}

void PixelEngine::update( float delta )
{
    delta = delta / 100'000;
    if( dynamic_count != 0 )
    {
        for( int i = dynamic_count-1; i >= 0; i-- )
        {
            if( !dynamic_alive[i] )
            {
                if( dynamic_count == i + 1 )
                {
                    dynamic_count = i;
                    if( dynamic_last_free > dynamic_count )
                    {
                        dynamic_last_free=dynamic_count;
                    }
                }
                continue;
            }

            if( dynamic_life_time[i] != -1 )
            {
                dynamic_life_time[i] -= delta;
                if( dynamic_life_time[i] < 0 )
                {
                    dynamic_alive[i] = false;
                    int x = dynamic_pos[i][0];
                    int y = dynamic_pos[i][1];
                    this->map_pixel[x][y] = false;
                    this->map_color[x][y][3] = 0;
                    continue;
                }
            }

            dynamic_vec[i][0] += 0.1;
            //dynamic_vec[i][1] *= -1;

            float nan = 0.0f / 0.0f;

            nan = -nan;

            if( dynamic_vec_to[i][0] != dynamic_vec_to[i][0] )
            {
                dynamic_vec_to[i][0] = 0;
                dynamic_vec_to[i][1] = 0;
            }

            if( dynamic_vec[i][0] != dynamic_vec[i][0] )
            {
                dynamic_vec[i][0] = 0;
                dynamic_vec[i][1] = 0;
            }

            int x = dynamic_pos[i][0];
            int y = dynamic_pos[i][1];
            float move_x = (dynamic_vec[i][0] * delta) + dynamic_vec_to[i][0];
            float move_y = (dynamic_vec[i][1] * delta) + dynamic_vec_to[i][1];
            dynamic_vec_to[i][0] = move_x - ((int)move_x);
            dynamic_vec_to[i][1] = move_y - ((int)move_y);
            int tx = x + int(move_x);
            int ty = y + int(move_y);

            int nx = x;
            int ny = y;

            this->map_pixel[x][y] = false;
            bool collide = this->castRay(nx,ny,tx,ty);
            this->map_pixel[x][y] = true;

            if( ny < -10 )
            {
                y = x;
            }

            dynamic_pos[i][0] = nx;
            dynamic_pos[i][1] = ny;

            if( nx != x ||
                ny != y )
            {
                this->map_pixel[x][y] = false;
                this->map_dynamic[x][y] = false;
                this->map_pixel[nx][ny] = true;
                this->map_dynamic[nx][ny] = true;

                this->map_dynamic_ptr[nx][ny] = i;

                this->map_color[nx][ny][0] = this->map_color[x][y][0];
                this->map_color[nx][ny][1] = this->map_color[x][y][1];
                this->map_color[nx][ny][2] = this->map_color[x][y][2];
                this->map_color[nx][ny][3] = 255;

                this->map_color[x][y][3] = 0;
            }

            if ( !collide )
            {
                
            }
            else
            {
                if( !this->map_dynamic[tx][ty] && this->map_pixel[tx][ty] )
                {
                    if( this->dynamic_life_time[i] == -1 &&
                        !(   this->dynamic_vec[i][0]*this->dynamic_vec[i][0] > 1 || 
                             this->dynamic_vec[i][1]*this->dynamic_vec[i][1] > 1) )
                    {
                        this->map_pixel[nx][ny] = true; 
                        this->map_dynamic[nx][ny] = false;
                        dynamic_alive[i] = false;
                        //this->map_color[nx][ny][0] = 78;
                        //this->map_color[nx][ny][1] = 78;
                        //this->map_color[nx][ny][2] = 78;
                        this->map_color[nx][ny][3] = 255;
                    }
                    else
                    {
                        float normal_x = nx;
                        float normal_y = ny;
                        this->get_normal( normal_x, normal_y );
                        if( normal_x != 0 &&
                            normal_y != 0 &&
                            normal_x != 0.0f/0.0f &&
                            normal_y != 0.0f/0.0f )
                        {
                            normal_x = fabs(normal_x);
                            normal_y = fabs(normal_y);
                            dynamic_vec[i][0] *= (1 - (normal_x * 2)) * 0.5;
                            dynamic_vec[i][1] *= (1 - (normal_y * 2)) * 0.5;
                        }
                    }
                }
                else
                {
                    if( this->map_pixel[tx][ty] && this->map_dynamic[tx][ty] )
                    {
                        float old_x_vec = dynamic_vec[i][0];
                        float old_y_vec = dynamic_vec[i][1];
                        dynamic_vec[i][0] = dynamic_vec[map_dynamic_ptr[tx][ty]][0];
                        dynamic_vec[i][1] = dynamic_vec[map_dynamic_ptr[tx][ty]][1];
                        dynamic_vec[map_dynamic_ptr[tx][ty]][0] = old_x_vec;
                        dynamic_vec[map_dynamic_ptr[tx][ty]][1] = old_y_vec;
                    }   
                }
            }
        }
    }
}

void PixelEngine::add_static_pixel( int x, int y, int r, int g, int b)
{
    this->map_pixel[x][y] = true;
    this->map_color[x][y][0] = r;
    this->map_color[x][y][1] = g;
    this->map_color[x][y][2] = b;
    this->map_color[x][y][3] = 255;
}

void PixelEngine::spam_powder( int x, int y )
{
    int px = 0;
    int py = 0;
    for( int i = -30; i < 30; i++ )
    {
        for( int j = -30; j < 30; j++ )
        {
            px = x + i;
            py = y + j;
            if( this->map_pixel[px][py] )
            {
                if( this->map_dynamic[px][py] )
                {

                }
                else
                {
                    //this->map_color[px][py][0] = 200;
                    //this->map_color[px][py][1] = 0;
                    //this->map_color[px][py][2] = 0;
                    this->map_color[px][py][3] = 255;
                    
                    this->map_dynamic[px][py] = true;

                    this->map_dynamic_ptr[px][py] = this->dynamic_last_free;
                    this->dynamic_life_time[dynamic_last_free] = -1;

                    this->dynamic_pos[dynamic_last_free][0] = px;
                    this->dynamic_pos[dynamic_last_free][1] = py;
                    this->dynamic_vec[dynamic_last_free][0] = float(i)/4;
                    this->dynamic_vec[dynamic_last_free][1] = float(j)/4;
                    this->dynamic_vec_to[dynamic_last_free][0] = 0;
                    this->dynamic_vec_to[dynamic_last_free][1] = 0;

                    this->dynamic_alive[this->dynamic_last_free] = true;
                    this->map_dynamic_ptr[px][py] = this->dynamic_last_free;

                    if( dynamic_last_free == dynamic_count )
                    {
                        dynamic_last_free++;
                        dynamic_count++;
                    }
                    else
                    {
                        while( !dynamic_alive[this->dynamic_last_free] )
                        {
                            dynamic_last_free++;
                        }
                        if( dynamic_last_free > dynamic_count )
                        {
                            dynamic_count = dynamic_last_free+1;
                        }
                    }
                }
            }
        }
    }
}

void PixelEngine::spam_dynamic_pixels( int x, int y, float life )
{
    for( int i = x; i < x+20; i++ )
    {
        for( int j = y; j < y+20; j++ )
        {
            if( this->map_pixel[i][j] )
            {
                continue;
            }
            this->map_color[i][j][0] = 0;
            this->map_color[i][j][1] = 0;
            this->map_color[i][j][2] = 255;
            this->map_color[i][j][3] = 255;
            this->map_pixel[i][j] = true;
            this->map_dynamic[i][j] = true;

            this->map_dynamic_ptr[i][j] = this->dynamic_last_free;

            this->dynamic_life_time[this->dynamic_last_free] = ((life == -1) ? -1 : life + rand() % 100);

            this->dynamic_pos[this->dynamic_last_free][0] = i;
            this->dynamic_pos[this->dynamic_last_free][1] = j;
            this->dynamic_vec[this->dynamic_last_free][0] = 1;
            this->dynamic_vec[this->dynamic_last_free][1] = float(150-rand()%301) / 100.0f;
            this->dynamic_vec_to[this->dynamic_last_free][0] = 0;
            this->dynamic_vec_to[this->dynamic_last_free][1] = 0;
            this->dynamic_alive[this->dynamic_last_free] = true;
            if( dynamic_last_free == dynamic_count )
            {
                dynamic_last_free++;
                dynamic_count++;
            }
            else
            {
                while( !dynamic_alive[this->dynamic_last_free] )
                {
                    dynamic_last_free++;
                }
                if( dynamic_last_free > dynamic_count )
                {
                    dynamic_count = dynamic_last_free+1;
                }
            }
        }
    }
}

void PixelEngine::draw()
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture( GL_TEXTURE_2D, glid );

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,map_size_w,map_size_h,0,GL_RGBA,GL_UNSIGNED_BYTE,this->map_color);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0); glVertex3f( 0, 
                                           0, 1 );
        glTexCoord2f(0.0,1.0); glVertex3f( 0, 
                                           -int(map_size_h) * to_screen_coff, 1 );
        glTexCoord2f(1.0,1.0); glVertex3f( map_size_w * to_screen_coff, 
                                           -int(map_size_h) * to_screen_coff, 1 );
        glTexCoord2f(1.0,0.0); glVertex3f( map_size_w * to_screen_coff, 
                                           0, 1 );
        /*glTexCoord2f(0.0,0.0); glVertex3f( 0,           0,                1 );
        glTexCoord2f(0.0,1.0); glVertex3f( 0,           -int(500), 1 );
        glTexCoord2f(1.0,1.0); glVertex3f( 500,  -int(500), 1 );
        glTexCoord2f(1.0,0.0); glVertex3f( 500,  0,                1 );*/
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

} // namespace re