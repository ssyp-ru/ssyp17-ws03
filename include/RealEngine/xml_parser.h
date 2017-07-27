#pragma once
#include <vector>
#include <map>

namespace re {
    struct XmlElem {
        // Logs are written to logs/xml_parser.txt

        std::string name;
        std::map<std::string, std::string> field;
        std::vector<XmlElem*> child;
        std::string content;
        XmlElem* parent;
        
        XmlElem();

        // Asquires every XmlElem from child with given name
        std::vector<XmlElem*> get_children(std::string);
        
        // prints XmlElem with its content to file
        void print(std::string output_filename);
    };

    // return XmlElem with name of file and with every elements found inside
    XmlElem parse_xml(std::string input_filename);
}