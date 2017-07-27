#include <RealEngine/xml_parser.h>
#include <RealEngine/tiled_parser.h>
#include <RealEngine/logger.h>

int main(){
    re::XmlElem e = re::parse_xml("example.xml");
    e.print("parsedExample.xml");
    try {
        re::parse_tiled(e)[0].print("testTiled.txt");
    } catch(...) {
        re::Log::msg("Could not print test!", re::Log::LEVEL::DEBUG);
        // INFO and TRACE levels are also avaiable
    }
}