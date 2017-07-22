#include "RealEngine/graphic/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

#include <iostream>

namespace re{

Image::Image(const std::string filename){
    image_buffer = stbi_load(filename.c_str(), &w, &h, &comp, 0);
    if (image_buffer == nullptr){
        std::cerr << "Image::Image(const std::string filename): fail to load image " << filename << std::endl;
    }
}

Image::Image(const Image& im){
    std::cerr << "incomplete copy constructor of image\n";
}

Image::Image(int width, int height, int c){
    image_buffer = (unsigned char *) malloc(width*height*c);
    w = width;
    h = height;
    comp = c;
}

Image::~Image(){
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


} // namespace re