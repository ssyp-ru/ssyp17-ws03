#pragma once
#include "xml_parser.h"
#include <RealEngine/graphic/image.h>

namespace re {

struct Object {
    uint id;
    std::string name;
    uint height, width;
    double x, y;
    std::vector<std::string> flags;
};

struct ObjectGroup {
    std::string name;
    std::vector<Object> group;
};

struct Tileset {
    uint firstgid;
    uint tilewidth, tileheight, tilecount, columns;
    uint img_width, img_height;
    std::string source;
    std::string name;
    std::string img_source;
    std::vector<ImagePtr> tiles;
};

struct Layer {
    uint height;
    uint width;
    std::string name;
    std::vector<uint> data;
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