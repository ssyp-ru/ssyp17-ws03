#pragma once

#include <RealEngine/xml_parser.h>
#include <RealEngine/math.h>

#include <memory>
#include <string>

namespace re {

// Manager for config. Config file - config.xml
class ConfigManager {
public:
    static std::string get_property(std::string path_to_property);

private:
    ConfigManager();
    ConfigManager& operator=(ConfigManager&);

    static ConfigManager& instance()
    {
        static ConfigManager self;
        return self;
    }

    XmlElemPtr root = nullptr;
};


} // namespace re