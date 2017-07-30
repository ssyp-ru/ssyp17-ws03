#include <RealEngine/config_manager.h>
#include <RealEngine/xml_parser.h>
#include <RealEngine/logger.h>

#include <string>

namespace re {

const std::string CONFIG_FILE_NAME = "config.xml";

ConfigManager::ConfigManager(){
    XmlElemPtr config_ptr = parse_xml(CONFIG_FILE_NAME);
    if (config_ptr){
        root = config_ptr->get_children("config")[0];
    } else {
        Log::msg("ConfigManager: Can't load config.xml", Log::LEVEL::INFO);
    }
}


std::string ConfigManager::get_property(std::string path_to_property) {
    if (instance().root == nullptr){
        return "";
    }
    XmlElemPtr elem = instance().root;
    auto l = path_to_property.find('/');
    while (l != std::string::npos) {
        elem = elem->get_children(path_to_property.substr(0, l))[0];
        path_to_property.erase(0, l + 1);
        l = path_to_property.find('/');
    }
    return elem->field[path_to_property];
}


}