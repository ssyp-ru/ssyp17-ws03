#pragma once
#include "xml_parser.h"
#include <RealEngine/graphic/image.h>

namespace re {

    struct Object {
        uint id;
        std::string name;
        uint height;
        uint width;
        double x;
        double y;
        std::vector<std::string> flags;
    };
    struct ObjectGroup {
        std::string name;
        std::vector<Object> group;
    };
    struct Tileset {
        uint firstgid;
        std::string source;
        std::string name;
        uint tilewidth;
        uint tileheight;
        uint tilecount;
        uint columns;
        std::string img_source;
        uint img_width;
        uint img_height;
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

    std::vector<Map> parse_tiled(XmlElemPtr parsed_xml);
}