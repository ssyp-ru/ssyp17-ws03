#include "xml_parser.h"
#include "../utils/logger.h"

re::Log log("logs/xmlLog.txt");

re::XmlElem::XmlElem()
{
    parent = nullptr;
}

std::vector<re::XmlElem*> re::XmlElem::getChildren(std::string name) {
    std::vector<re::XmlElem*> arr;
    log.track();
    for(auto it = child.begin(); it != child.end(); it++) {
        if((*it)->name == name) {
            arr.push_back(*it);
        }
    }
    log.msg(".getChildren on \"" + this->name + "\" took " + std::to_string(log.track_elapsed()) + " ns.");
    return arr;
}

void printXmlElemRec(re::XmlElem* elem, re::Log* out, int depth) {
    for(int i = 0; i < depth; i++) out->stream() << '\t';
    out->stream() << '<' << elem->name;
    auto itName = elem->fieldName.begin();
    auto itData = elem->fieldData.begin();
    while(itName != elem->fieldName.end() && itData != elem->fieldData.end()) {
        out->stream() << ' ' << *itName << '=' << *itData;
        ++itName;
        ++itData;
    }
    out->stream() << '>' << std::endl;
    for(auto it = elem->data.begin(); it != elem->data.end(); it++) {
        for(int i = 0; i <= depth; i++) out->stream() << '\t';
        out->stream() << *it << std::endl;
    }
    for(auto it = elem->child.begin(); it != elem->child.end(); it++) {
        printXmlElemRec(*it, out, depth+1);
    }
    for(int i = 0; i < depth; i++) out->stream() << '\t';
    out->stream() << "</" << elem->name << '>' << std::endl;
}

void re::printXmlElem(XmlElem elem) {
    Log* outlog = new Log("outXml.txt");
    log.track();
    printXmlElemRec(&elem, outlog, 0);
    log.msg("Printing \"" + elem.name + "\" took " + std::to_string(log.track_elapsed()) + " ns.");
    delete outlog;
}

enum READ_MODE {
    READ_NONE, READ_INSIDE, READ_NAME, READ_WAIT_VALUE, READ_VALUE, READ_CLOSE
};

re::XmlElem re::parseXmlFile(std::string filename) {
    std::ifstream in(filename);
    log.track();
    int lineN = 0;
    std::string buffer = "";
    READ_MODE rm = READ_NONE;
    XmlElem origin;
    origin.name = filename;
    if(in.is_open()) {
        log.msg("File \""+filename+"\" found!");
        XmlElem* curr = &origin;
        while(!in.eof()) {
            lineN++;
            std::string line;
            getline(in, line);
            log.msg("Read: \"" + line + "\"");
            bool hasMetEqualSign = false;
            for(int i = 0; i < line.length(); i++) {
                switch (rm) {
                    case READ_NONE:
                        if(line[i] == '\t') break; // hate tabs
                        if(buffer.length() <= 0 && line[i] == ' ') break; // hate tabs
                        if(line[i] == '<') {
                            if(buffer.length() > 0) {
                                curr->data.push_back(buffer);
                                buffer = "";
                            }
                            rm = READ_INSIDE;
                            XmlElem* n = new XmlElem();
                            n->parent = curr;
                            curr->child.push_back(n);
                            curr = n;
                        } else buffer += line[i];
                    break;
                    case READ_INSIDE:
                        switch(line[i]) {
                            case '/':
                                rm = READ_CLOSE;
                            break;
                            case '>':
                                rm = READ_NONE;
                            break;
                            case '=':
                                rm = READ_VALUE;
                            break;
                            case ' ':
                            case '\t':
                            case '\n':
                            break;
                            default:
                                buffer += line[i];
                                rm = READ_NAME;
                            break;
                        }
                    break;
                    case READ_NAME:
                        switch(line[i]) {
                            case '>':
                                if(buffer.length() > 0) {
                                    if(curr->name.length() > 0) {
                                        log.msg("On (" + std::to_string(lineN) + ", " + std::to_string(i) + "): renamed XmlElem.");
                                    }
                                    curr->name = buffer;
                                    buffer = "";
                                }
                                rm = READ_NONE;
                            break;
                            case ' ':
                            case '\t':
                            case '\n':
                                if(buffer.length() > 0) {
                                    if(curr->name.length() > 0) {
                                        curr->fieldName.push_back(buffer);
                                        buffer = "";
                                        hasMetEqualSign = false;
                                        rm = READ_WAIT_VALUE;
                                    } else {
                                        curr->name = buffer;
                                        buffer = "";
                                        rm = READ_INSIDE;
                                    }
                                }
                            break;
                            case '=':
                                if(buffer.length() > 0) {
                                    curr->fieldName.push_back(buffer);
                                    buffer = "";
                                } else {
                                    log.msg("On (" + std::to_string(lineN) + ", " + std::to_string(i) + "): empty field name found.");
                                }
                                rm = READ_VALUE;
                            break;
                            default:
                                buffer += line[i];
                            break;
                        }
                    break;
                    case READ_WAIT_VALUE:
                        switch(line[i]) {
                            case '>':
                                log.msg("On (" + std::to_string(lineN) + ", " + std::to_string(i) + "): expected a value.");
                            break;
                            case ' ':
                            case '\t':
                            case '\n':
                            case '=':
                            break;
                            default:
                                buffer += line[i];
                                rm = READ_VALUE;
                            break;
                        }
                    break;
                    case READ_VALUE:
                        switch(line[i]) {
                            case '>':
                                rm = READ_NONE;
                                if(buffer.length() > 0) {
                                    curr->fieldData.push_back(buffer);
                                    buffer = "";
                                } else {
                                    log.msg("On (" + std::to_string(lineN) + ", " + std::to_string(i) + "): empty field value found.");
                                }
                            case ' ':
                            case '\t':
                            case '\n':
                                if(rm == READ_VALUE) rm = READ_INSIDE;
                            break;
                            default:
                                buffer += line[i];
                            break;
                        }
                    break;
                    case READ_CLOSE:
                        if(line[i] != ' ' && line[i] != '\t' && line[i] != '\n'){
                            if(line[i] == '>') {
                                if(curr->parent->name != buffer) {
                                    log.msg("On (" + std::to_string(lineN) + ", " + std::to_string(i) + "): incorrect closing caret (comparing names "+curr->parent->name+" and "+buffer+").");
                                }
                                buffer = "";
                                XmlElem* t = curr;
                                curr = curr->parent;
                                curr->child.pop_back();
                                curr = curr->parent;
                                delete t;
                                rm = READ_NONE;
                            } else buffer += line[i];
                        }
                    break;
                    default:
                    break;
                }
            }
            if(rm == READ_NONE && buffer.length() > 0) {
                curr->data.push_back(buffer);
                buffer = "";
            }
        }
        in.close();
        log.msg("File closed successfully.");
    } else {
        log.msg("Could not open the file!");
    }
    log.msg("Parsing took " + std::to_string(log.track_elapsed()) + " ns.");
    return origin;
}