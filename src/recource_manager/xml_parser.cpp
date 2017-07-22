#include "xml_parser.h"

std::vector<re::XmlElem> re::parseXmlFile(std::string filename) {
    std::ifstream out(filename);
    char* line;
    size_t size;
    while(out.getline(line, size)) {

    }
}