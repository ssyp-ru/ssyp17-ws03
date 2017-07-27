#include <RealEngine/resource_manager.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/xml_parser.h>
#include <RealEngine/logger.h>

namespace re {

bool ResourceManager::load_file(std::string path_to_config_file) {
    XmlElem elem = parse_xml(path_to_config_file);
    std::vector<XmlElem *> images = elem.get_children("resources")[0]->get_children("images")[0]->get_children("im");
    std::vector<XmlElem *> sprites = elem.get_children("resources")[0]->get_children("sprites")[0]->get_children("sprite");
    for (auto ptr : images) {
        load_image_feature(ptr);
    }
    for (auto ptr : sprites) {
        load_sprite_feature(ptr);
    }
    return true;
}

ImagePtr ResourceManager::get_image(std::string feature_id) {
    if (image_storage.find(feature_id) == image_storage.end())
        return nullptr;
    return image_storage[feature_id];
}

std::shared_ptr<Animation> ResourceManager::get_animation(std::string feature_id) {
    if (animation_storage.find(feature_id) == animation_storage.end())
        return nullptr;
    return animation_storage[feature_id];
}

bool ResourceManager::load_image_feature(XmlElem *layer_ptr) {
    ImagePtr image = std::make_shared<Image>(layer_ptr->field["file"]);
    if (image == nullptr)
        return false;
    image_storage[layer_ptr->field["id"]] = image;
    return true;
}

bool ResourceManager::load_sprite_feature(XmlElem *sprite_ptr) {
    Animation anim(std::stof(sprite_ptr->field["speed"]), sprite_ptr->field["cycleable"] == "true");
    ImagePtr image = get_image(sprite_ptr->field["source_id"]);
    if (image == nullptr)
        return false;
    for (auto sp : sprite_ptr->get_children("sp")) {
        int x, y, width, height;
        try {
            x = std::stoi(sp->field["x"]);
            y = std::stoi(sp->field["y"]);
            width = std::stoi(sp->field["w"]);
            height = std::stoi(sp->field["h"]);
        } catch (std::invalid_argument) {
            return false;
        }
        anim.add_frame(image->get_subimage(x, y, width, height));
    }
    animation_storage[sprite_ptr->field["id"]] = std::make_shared<Animation>(anim);
    return true;
}

} //namespace re