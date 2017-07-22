#pragma once
#include <vector>
#include <fstream>

namespace re {
    struct XmlElem {
        /*
            <name fieldName=fieldData>
                <child>
                data
                <child>
                data
            </name>
        */
        std::string name;
        std::vector<std::string> fieldName;
        std::vector<std::string> fieldData;
        std::vector<XmlElem> child;
        std::vector<std::string> data;
    };

    std::vector<XmlElem> parseXmlFile(std::string filename);
}