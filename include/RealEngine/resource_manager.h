#include <RealEngine/graphic/animation.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/xml_parser.h>

#include <string>

namespace re {

class ResourceManager {
public:
    bool load_file(std::string path_to_config_file);

    ImagePtr get_image(std::string feature_id);
    AnimationPtr get_animation(std::string feature_id);

private:
    bool load_image_feature(XmlElem *layer_ptr);
    bool load_sprite_feature(XmlElem *sprite_ptr);

    std::map<std::string, ImagePtr> image_storage;
    std::map<std::string, AnimationPtr> animation_storage;
};

} // namespace re