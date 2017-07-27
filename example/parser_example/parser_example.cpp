#include <RealEngine/xml_parser.h>
#include <RealEngine/tiled_parser.h>
#include <RealEngine/logger.h>

int main(){
    re::XmlElem e = re::parse_xml("example.xml");
    e.print("parsedExample.xml");
    try {
        re::Map map = re::parse_tiled(e)[0];
        std::ofstream out("testTiled.txt");
        out << "start: " << std::endl;
        out << map.tileset.firstgid << std::endl;
        out << map.tileset.source << std::endl;
        out << map.layer.name << std::endl;
        out << map.layer.height << std::endl;
        out << map.layer.width << std::endl;
        for(auto& i : map.layer.data) {
            out << i;
        }
        out << std::endl;
        out << map.objectgroup.name << std::endl;
        for(auto& i : map.objectgroup.group) {
            out << '(' << i.id << ')' << i.name << '(' << i.x << ',' << i.y << ')' << i.width << '/' << i.height;
            for(auto& j : i.flags) {
                out << ';' << j;
            }    
            out << std::endl;
        }

    } catch(...) {
        re::Log::msg("Could not print test!", re::Log::LEVEL::DEBUG);
        // INFO and TRACE levels are also avaiable
    }
}