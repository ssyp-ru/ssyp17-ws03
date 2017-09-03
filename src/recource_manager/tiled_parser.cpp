#include <RealEngine/tiled_parser.h>
#include <RealEngine/logger.h>
#include <RealEngine/graphic/image.h>

#include <iostream>

namespace re{


Object parse_object(XmlElemPtr parsed_xml) {
    Log::msg("parsing to object " + parsed_xml->name, Log::LEVEL::TRACE);    
    Object object;
    try {
        object.name = parsed_xml->field.at("name");
        object.id = std::stoi(parsed_xml->field.at("id"));
        object.height = std::stoi(parsed_xml->field.at("height"));
        object.width = std::stoi(parsed_xml->field.at("width"));
        object.x = std::stod(parsed_xml->field.at("x"));
        object.y = std::stod(parsed_xml->field.at("y"));
        for(auto& obj : parsed_xml->child) {
            object.flags.push_back(obj->name);
        }
    } catch(...) {
        Log::msg("Error parsing " + parsed_xml->name + " as an object.", Log::LEVEL::DEBUG);
    }
    return object;
}


ObjectGroup parse_objectgroup(XmlElemPtr parsed_xml) {
    Log::msg("parsing to objectgroup " + parsed_xml->name, Log::LEVEL::TRACE);    
    ObjectGroup objectgroup;
    try {
        objectgroup.name = parsed_xml->field.at("name");
        for(auto& obj : parsed_xml->child) {
            objectgroup.group.push_back(parse_object(obj));
        }
    } catch(...) {
        Log::msg("Error parsing " + parsed_xml->name + " as an objectgroup.", Log::LEVEL::DEBUG);
    }
    return objectgroup;
}


Layer parse_layer(XmlElemPtr parsed_xml) {
    Log::msg("parsing to layer " + parsed_xml->name, Log::LEVEL::TRACE);
    Layer layer;
    try {
        layer.height = std::stoi(parsed_xml->field.at("height"));
        layer.width = std::stoi(parsed_xml->field.at("width"));
        layer.name = parsed_xml->field.at("name");
        XmlElemPtr data_xml = parsed_xml->child.at(0);
        std::string endcoding = data_xml->field.at("encoding");
        std::string data = data_xml->content;
        data.erase(0,1);
        while(data.length() > 0) {
            size_t found = data.find_first_not_of("\r ,\t\n");
            if(found != data.npos) {
                data.erase(0,found);
            }
            if(data.length() <= 0) break;
            found = data.find_first_of("\r ,\n\t");
            layer.data.push_back(std::stoi(data.substr(0,found)));
            data.erase(0,found);
        }
        Log::stream() << std::endl;
    } catch(...) {
        Log::msg("Error parsing " + parsed_xml->name + " \""+layer.name+"\" as a layer.", Log::LEVEL::DEBUG);
    }
    return layer;
}


Tileset parse_tileset(XmlElemPtr parsed_xml) {
    Log::msg("parsing to tileset " + parsed_xml->name, Log::LEVEL::TRACE);    
    Tileset tileset;
    try {
        tileset.firstgid = std::stoi(parsed_xml->field.at("firstgid"));
        tileset.source = parsed_xml->field.at("source");
        XmlElemPtr src = parse_xml(tileset.source);
        XmlElemPtr ts = src->get_children("tileset").at(0);
        tileset.name = ts->field.at("name");
        tileset.tilewidth = std::stoi(ts->field.at("tilewidth"));
        tileset.tileheight = std::stoi(ts->field.at("tileheight"));
        tileset.tilecount = std::stoi(ts->field.at("tilecount"));
        tileset.columns = std::stoi(ts->field.at("columns"));
        ts = ts->get_children("image").at(0);
        Log::msg("image : " + ts->name + ", source="+ts->field["source"], Log::LEVEL::TRACE);
        tileset.img_source = ts->field.at("source"); // this is filename
        tileset.img_width = std::stoi(ts->field.at("width"));
        tileset.img_height = std::stoi(ts->field.at("height"));
        auto tileset_src = std::make_shared<Image>(tileset.img_source);
        for(unsigned int j = 0; j < tileset.tilecount*tileset.columns; j++) {
            for(unsigned int i = 0; i < tileset.columns; i++) {
                tileset.tiles.push_back(tileset_src);//->get_subimage(i*tileset.tilewidth,j*tileset.tileheight,tileset.tilewidth,tileset.tileheight));
            }
        }
    } catch(...) {
        Log::msg("Error parsing " + parsed_xml->name + " as a tileset.", Log::LEVEL::DEBUG);
    }
    return tileset;
}


Map parse_map(XmlElemPtr parsed_xml) {
    Log::msg("parsing to map " + parsed_xml->name, Log::LEVEL::TRACE);
    Map map;
    try {
        for(auto& xml : parsed_xml->get_children("tileset")) {
            map.tileset.push_back(parse_tileset(xml));
        }
        for(auto& xml : parsed_xml->get_children("layer")) {
            map.layer.push_back(parse_layer(xml));
        }
        for(auto& xml : parsed_xml->get_children("objectgroup")) {
            map.objectgroup.push_back(parse_objectgroup(xml));
        }
    } catch(...) {
        Log::msg("Error parsing " + parsed_xml->name + " as a map.", Log::LEVEL::DEBUG);
    }
    for(auto& layer : map.layer) {
        try {
            auto width = map.tileset[0].tilewidth;
            auto height = map.tileset[0].tileheight;
            layer.background = std::make_shared<Image>(layer.width*width,layer.height*height, 4);

            for(unsigned int y = 0; y < layer.height; y++)
                for(unsigned int x = 0; x < layer.width; x++) {
                    int a = map.layer[0].data[y*layer.width + x];
                    if( a > 0 && a < 100 )
                    {
                        layer.background->set_subimage(x*width,y*height,
                            map.tileset[
                                a-1
                            ].tiles[0]);
                    }
                }
            layer.background->reGenTexture();
        } catch(...) {
            Log::msg("Error creating background image for layer " + layer.name + ".", Log::LEVEL::DEBUG);
        }
    }
    return map;
}


std::vector<Map> parse_tiled(XmlElemPtr parsed_xml) {
    std::vector<Map> maps;
    for(auto& xml_map : parsed_xml->get_children("map")) {
        maps.push_back(parse_map(xml_map));
    }
    return maps;
}


std::vector<Map> get_tmx_map(const std::string& filepath){
    return (re::parse_tiled( re::parse_xml( filepath ) ));
}

} // namespace re