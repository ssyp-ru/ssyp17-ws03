#pragma once

#include "stdint.h"

namespace re {

struct Color 
{
    uint8_t r, g, b, t;

    Color() : r(0), g(0), b(0), t(0) {}

    Color(uint8_t grayscale, uint8_t trans = 255) 
        : r(grayscale)
        , g(grayscale)
        , b(grayscale)
        , t(trans)      {}

    Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t t_ = 255) 
        : r(r_)
        , g(g_)
        , b(b_)
        , t(t_)         {}

    int to_int(){
        return ((int)t << 24) | ((int)r << 16) | ((int)g << 8) | (int)b;
    }
};

static const Color WHITE    (0xff, 0xff, 0xff);
static const Color BLACK    (0x00, 0x00, 0x00);
static const Color RED      (0xff, 0x00, 0x00);
static const Color GREEN    (0x00, 0xff, 0x00);
static const Color BLUE     (0x00, 0x00, 0xff);


} // namespace re