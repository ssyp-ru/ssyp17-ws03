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
        std::vector<XmlElem*> child;
        std::vector<std::string> data;
        XmlElem* parent;
        
        XmlElem();

        // Asquires every XmlElem from child with given name
        std::vector<XmlElem*> getChildren(std::string);
    };

    XmlElem parseXmlFile(std::string filename);
    void printXmlElem(XmlElem);
}