#include "RealEngine/graphic/image.h"

// #include <GL/gl.h>
// #include <GL/glu.h>
#include <GL/glut.h>
// #include <GL/glx.h>



#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

#include <iostream>
//#include <swap>

namespace re{

Image::Image(const std::string filename){
    image_buffer = stbi_load(filename.c_str(), &w, &h, &comp, 0);
    if (image_buffer == nullptr){
        std::cerr << "Image::Image(const std::string filename): fail to load image " << filename << std::endl;
    }
    
    glGenTextures( 1, &glid );
    glBindTexture( GL_TEXTURE_2D, glid );

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,image_buffer);
}

Image::Image(const Image& im){
    std::cerr << "incomplete copy constructor of image\n";
}

Image::Image( void *buffer, int width, int height, int c )
{
    image_buffer = (unsigned char *)buffer;
    w = width;
    h = height;
    comp = c;

    glGenTextures( 1, &glid );
    glBindTexture( GL_TEXTURE_2D, glid );

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,image_buffer);
}

Image::Image(int width, int height, int c){
    image_buffer = (unsigned char *) malloc(width*height*c);
    w = width;
    h = height;
    comp = c;
    
    glGenTextures( 1, &glid );
    glBindTexture( GL_TEXTURE_2D, glid );

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,image_buffer);
}

Image::~Image(){
    GLuint tex = glid;
    glDeleteTextures( 2, &tex );
    free(image_buffer);
}

Color Image::get_pix_color(int x, int y){
    Color newColor;
    newColor.r = image_buffer[(((w * comp) * y) + x * comp) + 0]; 
    newColor.g = image_buffer[(((w * comp) * y) + x * comp) + 1]; 
    newColor.b = image_buffer[(((w * comp) * y) + x * comp) + 2];
    if (comp > 3)
        newColor.t = image_buffer[(((w * comp) * y) + x * comp) + 3]; 
    else 
        newColor.t = 255;
    return newColor;
}

ImagePtr Image::get_subimage(int x, int y, int size_x, int size_y){
    ImagePtr result = std::make_shared<Image>(size_x, size_y, comp);
    for (int i = x; i < x + size_x; i++){
        for (int j = y; j < y + size_y; j++){
            result->set_pix_color(i - x, j - y, get_pix_color(i, j));
        }
    }
    return result;
}

void Image::set_pix_color(int x, int y, Color c){
    image_buffer[(((w * comp) * y) + x * comp) + 0] = c.r;
    image_buffer[(((w * comp) * y) + x * comp) + 1] = c.g;
    image_buffer[(((w * comp) * y) + x * comp) + 2] = c.b;
    if (comp > 3)
        image_buffer[(((w * comp) * y) + x * comp) + 3] = c.t;
}

int Image::getTex()
{
    return glid;
}


} // namespace re