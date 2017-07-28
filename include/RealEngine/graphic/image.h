#pragma once

#include "color.h"

#include <string>
#include <vector>
#include <memory>

namespace re{

class Image
{
public:
    Image(std::string filename);
    Image(int width, int height, int c);
    Image( void *buffer, int width, int height, int c );
    ~Image();

    std::shared_ptr<Image> get_subimage(int x, int y, int size_x, int size_y);

    int getTex();

    Color get_pix_color(int x, int y);
    void set_pix_color(int x, int y, Color c);
    void set_subimage(int x, int y, std::shared_ptr<Image> im);

    void reGenTexture();

    int w, h, comp;

private:
    unsigned char* image_buffer;
    bool gl;
    uint glid;

    void generate_texture();
};
typedef std::shared_ptr<Image> ImagePtr;


} // namespace re