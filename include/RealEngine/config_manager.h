#include <RealEngine/xml_parser.h>
#include <RealEngine/math.h>

#include <memory>
#include <string>

namespace re {


class ConfigManager {
private:
    XmlElem *root;

public:
    bool load_file(std::string path_to_config_file);
    std::string get_property(std::string path_to_property);
};


} // namespace re