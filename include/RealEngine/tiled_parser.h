#pragma once
#include "xml_parser.h"
#include <RealEngine/graphic/image.h>

namespace re {

struct Object {
    unsigned int id;
    std::string name;
    unsigned int height, width;
    double x, y;
    std::vector<std::string> flags;
};

struct ObjectGroup {
    std::string name;
    std::vector<Object> group;
};

struct Tileset {
    unsigned int firstgid;
    unsigned int tilewidth, tileheight, tilecount, columns;
    unsigned int img_width, img_height;
    std::string source;
    std::string name;
    std::string img_source;
    std::vector<ImagePtr> tiles;
};

struct Layer {
    unsigned int height;
    unsigned int width;
    std::string name;
    std::vector<unsigned int> data;
    ImagePtr background;
};
struct Map {
    std::vector<Tileset> tileset;
    std::vector<Layer> layer;
    std::vector<ObjectGroup> objectgroup;
};

std::vector<Map> get_tmx_map(const std::string& filepath);
std::vector<Map> parse_tiled(XmlElemPtr parsed_xml);

}