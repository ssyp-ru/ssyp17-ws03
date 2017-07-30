#include <RealEngine/resource_manager.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/xml_parser.h>
#include <RealEngine/logger.h>

#include <string>

namespace re {


bool ResourceManager::load_file(std::string path_to_config_file) {
    XmlElemPtr root = parse_xml(path_to_config_file);
    if (root == nullptr){
        return false;
    }

    XmlElemPtr resources_ptr = root->get_children("resources")[0];
    if (resources_ptr == nullptr){
        return false;
    }

    // load images
    XmlElemPtr image_ptr = resources_ptr->get_children("images")[0];
    if (image_ptr != nullptr){
        std::vector<XmlElemPtr> images = image_ptr->get_children("im");
        for (auto ptr : images) {
            load_image_feature(ptr);
        }
    }
    
    // load sprites
    XmlElemPtr sprite_ptr = resources_ptr->get_children("sprites")[0];
    if (sprite_ptr != nullptr){
        std::vector<XmlElemPtr> sprites = sprite_ptr->get_children("sprite");
        for (auto ptr : sprites) {
            load_sprite_feature(ptr);
        }
    }

    return true;
}


ImagePtr ResourceManager::get_image(std::string feature_id) {
    if (image_storage.find(feature_id) == image_storage.end())
        return nullptr;
    return image_storage[feature_id];
}


AnimationPtr ResourceManager::get_animation(std::string feature_id) {
    if (animation_storage.find(feature_id) == animation_storage.end())
        return nullptr;
    return animation_storage[feature_id];
}


bool ResourceManager::load_image_feature(XmlElemPtr layer_ptr) {
    ImagePtr image = std::make_shared<Image>(layer_ptr->field["file"]);
    if (image == nullptr) {
        Log::msg("RM: Failed to load image '" + layer_ptr->field["id"] + "': no such file", Log::LEVEL::INFO);
        return false;
    }
    image_storage[layer_ptr->field["id"]] = image;
    return true;
}


bool ResourceManager::load_sprite_feature(XmlElemPtr sprite_ptr) {
    float anim_speed;
    std::string cycleable;
    try {
        anim_speed = std::stof(sprite_ptr->field["speed"]);
        cycleable  = sprite_ptr->field["cycleable"];
        if (cycleable != "true" && cycleable != "false")
            throw std::invalid_argument("");
    } catch (std::invalid_argument) {
        Log::msg("RM: Failed to load sprite '" + sprite_ptr->field["id"] + "': invalid arguments", Log::LEVEL::INFO);
        return false;
    }
    Animation anim(anim_speed, cycleable == "true");
    ImagePtr image = get_image(sprite_ptr->field["source_id"]);
    if (image == nullptr) {
        Log::msg("RM: Failed to load sprite '" + sprite_ptr->field["id"] + "': no such spritesheet '" + sprite_ptr->field["source_id"] + "'", Log::LEVEL::INFO);
        return false;
    }
    for (auto sp : sprite_ptr->get_children("sp")) {
        int x, y, width, height;
        try {
            x = std::stoi(sp->field["x"]);
            y = std::stoi(sp->field["y"]);
            width = std::stoi(sp->field["w"]);
            height = std::stoi(sp->field["h"]);
        } catch (std::invalid_argument) {
            Log::msg("RM: Failed to load sprite '" + sprite_ptr->field["id"] + "' frame: invalid arguments", Log::LEVEL::INFO);
            return false;
        }
        anim.add_frame(image->get_subimage(x, y, width, height));
    }
    animation_storage[sprite_ptr->field["id"]] = std::make_shared<Animation>(anim);
    return true;
}


} // namespace re