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

static const Color WHITE     (255, 255, 255);
static const Color BLACK     (0, 0, 0);
static const Color RED       (255, 0, 0);
static const Color GREEN     (0, 255, 0);
static const Color BLUE      (0, 0, 255);
static const Color YELLOW    (255, 255, 0);
static const Color GRAY      (127, 127, 127);
static const Color LIGHTGRAY (191, 191, 191);
static const Color DARKGRAY  (64, 64, 64);
static const Color LIGHTRED  (255, 127, 127);
static const Color LIGHTGREEN(127, 255, 127);
static const Color LIGHTBLUE (127, 127, 255);
static const Color DARKRED   (127, 0, 0);
static const Color DARKGREEN (0, 127, 0);
static const Color DARKBLUE  (0, 0, 127);
static const Color TRANSPARENTBLACK(0, 0, 0, 127);


} // namespace re