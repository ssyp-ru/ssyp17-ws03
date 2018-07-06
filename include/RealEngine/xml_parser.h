#pragma once
#include <vector>
#include <map>
#include <memory>

namespace re {


struct XmlElem {
    // Logs are written to logs/xml_parser.txt
    typedef std::shared_ptr<XmlElem> XmlElemPtr;


    std::string name;
    std::map<std::string, std::string> field;
    std::vector<XmlElemPtr> child;
    std::string content;
    XmlElemPtr parent;
    
    XmlElem();

    // Asquires every XmlElem from child with given name
    std::vector<XmlElemPtr> get_children(std::string);
    
    // prints XmlElem with its content to file
    void print(std::string output_filename);

    std::string get_field_value(const std::string& field_name);
};
typedef std::shared_ptr<XmlElem> XmlElemPtr;

// return XmlElem with name of file and with every elements found inside
XmlElemPtr parse_xml(std::string input_filename);


} // namespace re