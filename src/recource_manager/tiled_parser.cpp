#include <RealEngine/tiled_parser.h>
#include <RealEngine/logger.h>
#include <RealEngine/graphic/image.h>

#include <iostream>

re::Object parse_object(re::XmlElem* parsed_xml) {
    re::Log::msg("parsing to object " + parsed_xml->name, re::Log::LEVEL::TRACE);    
    re::Object object;
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
        re::Log::msg("Error parsing " + parsed_xml->name + " as an object.", re::Log::LEVEL::DEBUG);
    }
    return object;
}
re::ObjectGroup parse_objectgroup(re::XmlElem* parsed_xml) {
    re::Log::msg("parsing to objectgroup " + parsed_xml->name, re::Log::LEVEL::TRACE);    
    re::ObjectGroup objectgroup;
    try {
        objectgroup.name = parsed_xml->field.at("name");
        for(auto& obj : parsed_xml->child) {
            objectgroup.group.push_back(parse_object(obj));
        }
    } catch(...) {
        re::Log::msg("Error parsing " + parsed_xml->name + " as an objectgroup.", re::Log::LEVEL::DEBUG);
    }
    return objectgroup;
}
re::Layer parse_layer(re::XmlElem* parsed_xml) {
    re::Log::msg("parsing to layer " + parsed_xml->name, re::Log::LEVEL::TRACE);
    re::Layer layer;
    try {
        layer.height = std::stoi(parsed_xml->field.at("height"));
        layer.width = std::stoi(parsed_xml->field.at("width"));
        layer.name = parsed_xml->field.at("name");
        re::XmlElem* data_xml = parsed_xml->child.at(0);
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
        re::Log::stream() << std::endl;
    } catch(...) {
        re::Log::msg("Error parsing " + parsed_xml->name + " \""+layer.name+"\" as a layer.", re::Log::LEVEL::DEBUG);
    }
    return layer;
}

re::Tileset parse_tileset(re::XmlElem* parsed_xml) {
    re::Log::msg("parsing to tileset " + parsed_xml->name, re::Log::LEVEL::TRACE);    
    re::Tileset tileset;
    try {
        tileset.firstgid = std::stoi(parsed_xml->field.at("firstgid"));
        tileset.source = parsed_xml->field.at("source");
        re::XmlElem src = re::parse_xml(tileset.source);
        re::XmlElem* ts = src.get_children("tileset").at(0);
        tileset.name = ts->field.at("name");
        tileset.tilewidth = std::stoi(ts->field.at("tilewidth"));
        tileset.tileheight = std::stoi(ts->field.at("tileheight"));
        tileset.tilecount = std::stoi(ts->field.at("tilecount"));
        tileset.columns = std::stoi(ts->field.at("columns"));
        ts = ts->get_children("image").at(0);
        re::Log::msg("image : " + ts->name + ", source="+ts->field["source"], re::Log::LEVEL::TRACE);
        tileset.img_source = ts->field.at("source");
        tileset.img_width = std::stoi(ts->field.at("width"));
        tileset.img_height = std::stoi(ts->field.at("height"));
        auto tileset_src = std::make_shared<re::Image>(tileset.img_source);
        for(uint j = 0; j < tileset.tilecount*tileset.columns; j++) {
            for(uint i = 0; i < tileset.columns; i++) {
                tileset.tiles.push_back(tileset_src);//->get_subimage(i*tileset.tilewidth,j*tileset.tileheight,tileset.tilewidth,tileset.tileheight));
            }
        }
    } catch(...) {
        re::Log::msg("Error parsing " + parsed_xml->name + " as a tileset.", re::Log::LEVEL::DEBUG);
    }
    return tileset;
}

re::Map parse_map(re::XmlElem* parsed_xml) {
    re::Log::msg("parsing to map " + parsed_xml->name, re::Log::LEVEL::TRACE);
    re::Map map;
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
        re::Log::msg("Error parsing " + parsed_xml->name + " as a map.", re::Log::LEVEL::DEBUG);
    }
    for(auto& layer : map.layer) {
        try {
            auto width = map.tileset[0].tilewidth;
            auto height = map.tileset[0].tileheight;
            layer.background = std::make_shared<re::Image>(layer.width*width,layer.height*height, 4);

            for(uint y = 0; y < layer.height; y++)
                for(uint x = 0; x < layer.width; x++) {
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
            re::Log::msg("Error creating background image for layer " + layer.name + ".", re::Log::LEVEL::DEBUG);
        }
    }
    return map;
}

std::vector<re::Map> re::parse_tiled(re::XmlElem parsed_xml) {
    std::vector<re::Map> maps;
    for(auto& xml_map : parsed_xml.get_children("map")) {
        maps.push_back(parse_map(xml_map));
    }
    return maps;
}