#include <RealEngine/config_manager.h>
#include <RealEngine/xml_parser.h>

#include <string>

namespace re {


bool ConfigManager::load_file(std::string path_to_config_file) {
    root = parse_xml(path_to_config_file)->get_children("config")[0];
    return true;
}


std::string ConfigManager::get_property(std::string path_to_property) {
    XmlElemPtr elem = root;
    auto l = path_to_property.find('/');
    while (l != std::string::npos) {
        elem = elem->get_children(path_to_property.substr(0, l))[0];
        path_to_property.erase(0, l + 1);
        l = path_to_property.find('/');
    }
    return elem->field[path_to_property];
}


}