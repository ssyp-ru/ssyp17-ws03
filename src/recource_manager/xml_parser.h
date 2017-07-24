#pragma once
#include <vector>
#include <fstream>

namespace re {
    struct XmlElem {
        // Logs are written to logs/xml_parser.txt

        std::vector<std::string> tag; // tag[0] == name, probably
        std::vector<std::string> fieldName;
        std::vector<std::string> fieldData;
        std::vector<XmlElem*> child;
        std::vector<std::string> data;
        XmlElem* parent;
        
        XmlElem();

        // returns if elem has tag
        bool hasTag(std::string);

        // Asquires every XmlElem from child with given name
        std::vector<XmlElem*> getChildren(std::string);
        void print(std::string outputFile);
    };

    // return XmlElem with name of file and with every elements found inside
    XmlElem parseXml(std::string inputFile);
    /* The struct example that parser can read with  
        <name fieldName = fieldData
                fieldName = fieldData>
            < child fieldName=fieldData >
            data
            <child  fieldName = fieldData>
            data
        </name>
    */

    void printXmlElem(XmlElem);

}