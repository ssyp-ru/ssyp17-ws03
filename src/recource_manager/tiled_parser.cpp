#include <RealEngine/tiled_parser.h>
#include <RealEngine/logger.h>
#include <fstream>

void re::Map::print(std::string output_filename) {
    std::ofstream out(output_filename);
    out << "start: " << std::endl;
    out << tileset.firstgid << std::endl;
    out << tileset.source << std::endl;
    out << layer.name << std::endl;
    out << layer.height << std::endl;
    out << layer.width << std::endl;
    for(auto& i : layer.data) {
        out << i;
    }
    out << std::endl;
    out << objectgroup.name << std::endl;
    for(auto& i : objectgroup.group) {
        out << '(' << i.id << ')' << i.name << '(' << i.x << ',' << i.y << ')' << i.width << '/' << i.height;
        for(auto& j : i.flags) {
            out << ';' << j;
        }    
        out << std::endl;
    }
}
re::Object parse_object(re::XmlElem* parsed_xml) {
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
    re::Layer layer;
    try {
        layer.height = std::stoi(parsed_xml->field.at("height"));
        layer.width = std::stoi(parsed_xml->field.at("width"));
        layer.name = parsed_xml->field.at("name");
        re::XmlElem* data_xml = parsed_xml->child.at(0);
        std::string endcoding = data_xml->field.at("encoding");
        std::string data = data_xml->content;
        while(data.length() > 0) {
            size_t found = data.find_first_not_of(" ,\n\t");
            data.erase(0,found);
            if(data.length() <= 0) break;
            found = data.find_first_of(" ,\n\t");
            layer.data.push_back(std::stoi(data.substr(0,found)));
            data.erase(0,found);
        }
    } catch(...) {
        re::Log::msg("Error parsing " + parsed_xml->name + " \""+layer.name+"\" as a layer.", re::Log::LEVEL::DEBUG);
    }
    return layer;
}
re::Tileset parse_tileset(re::XmlElem* parsed_xml) {
    re::Tileset tileset;
    try {
        tileset.firstgid = std::stoi(parsed_xml->field.at("firstgid"));
        tileset.source = parsed_xml->field.at("source");
    } catch(...) {
        re::Log::msg("Error parsing " + parsed_xml->name + " as a tileset.", re::Log::LEVEL::DEBUG);        
    }
    return tileset;
}

re::Map parse_map(re::XmlElem* parsed_xml) {
    re::Map map;
    try {
        map.tileset = parse_tileset(parsed_xml->get_children("tileset").at(0));
        map.layer = parse_layer(parsed_xml->get_children("layer").at(0));
        map.objectgroup = parse_objectgroup(parsed_xml->get_children("objectgroup").at(0));
    } catch(...) {
        re::Log::msg("Error parsing " + parsed_xml->name + " as a map.", re::Log::LEVEL::DEBUG);        
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