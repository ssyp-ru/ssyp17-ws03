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
    Image(const Image& im);
    Image(int width, int height, int c);
    ~Image();

    unsigned char* get_buffer() { return image_buffer; } // TODO. TMP debug!

    std::shared_ptr<Image> get_subimage(int x, int y, int size_x, int size_y);

    Color get_pix_color(int x, int y);
    void set_pix_color(int x, int y, Color c);
    int w, h, comp;

private:
    unsigned char* image_buffer;
};
typedef std::shared_ptr<Image> ImagePtr;


} // namespace re