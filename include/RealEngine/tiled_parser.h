#pragma once
#include "xml_parser.h"

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
    };
    struct Layer {
        uint height;
        uint width;
        std::string name;
        std::vector<uint> data;
    };
    struct Map {
        Tileset tileset;
        Layer layer;
        ObjectGroup objectgroup;
    };

    std::vector<Map> parse_tiled(XmlElem parsed_xml);
}