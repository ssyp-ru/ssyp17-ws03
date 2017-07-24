#include "xml_parser.h"
#include "../utils/logger.h"
#include "../utils/stop_watch.h"

re::Log log("logs/xml_parser.txt");

re::XmlElem::XmlElem()
{
    parent = nullptr;
}

bool re::XmlElem::hasTag(std::string name) {
    for(std::string str : tag) {
        if(str == name) return true;
    }
    return false;
}
std::vector<re::XmlElem*> re::XmlElem::getChildren(std::string name) {
    std::vector<re::XmlElem*> arr;
    StopWatch stopwatch;
    for(auto it = child.begin(); it != child.end(); it++) {
        if((*it)->hasTag(name)) arr.push_back(*it);
    }
    log.msg(".getChildren on \""+this->tag[0]+"\" took "
        +std::to_string(stopwatch.stop_watch())+" ns.");
    return arr;
}

void printXmlElemRec(re::XmlElem* elem, re::Log* out, int depth) {
    for(int i = 0; i < depth; i++) out->stream() << '\t';
    out->stream() << "< ";
    
    for(std::string str : elem->tag) out->stream() << str << ' ';
    auto itName = elem->fieldName.begin();
    auto itData = elem->fieldData.begin();
    while(itName != elem->fieldName.end() && itData != elem->fieldData.end()) {
        out->stream() << *itName << " = " << *itData << ' ';
        ++itName;
        ++itData;
    }
    out->stream() << '>' << std::endl;
    for(std::string str : elem->data) {
        for(int i = 0; i <= depth; i++) out->stream() << '\t';
        out->stream() << str << std::endl;
    }
    for(re::XmlElem* e : elem->child) printXmlElemRec(e, out, depth+1);
    for(int i = 0; i < depth; i++) out->stream() << '\t';
    out->stream() << "</" << elem->tag[0] << '>' << std::endl;
}

void re::XmlElem::print(std::string outputFile) {
    Log* outlog = new Log(outputFile);
    StopWatch stopwatch;
    printXmlElemRec(this, outlog, 0);
    log.msg("Printing \""+tag[0]+"\" took "+std::to_string(stopwatch.stop_watch()/1000000)+" ms.");
    delete outlog;
}

enum READ_MODE {
    READ_NONE, READ_NAME, READ_WAIT_VALUE, READ_VALUE, READ_CLOSE
};

re::XmlElem re::parseXml(std::string filename) {
    std::ifstream in(filename);
    
    XmlElem origin;
    origin.tag.push_back(filename);
    if(!in.is_open()) {
        log.msg("Could not open the file!");
        return origin;
    }
    log.msg("File \""+filename+"\" found!");
    StopWatch stopwatch; // recording the time for 'now'
    XmlElem* current = &origin; // current object reading now
    READ_MODE read_mode = READ_NONE; 
    std::string buffer = "";
    int lineN = 0; // Counter for lines used only for logging
    while(!in.eof()) {
        std::string line;
        getline(in, line);
        log.msg("Read line #"+std::to_string(++lineN)+": \""+line+"\"");
        for(int i = 0; i < line.length(); i++) {
            switch (read_mode) {
                case READ_NONE:
                    if(line[i] == '<') {
                        if(buffer.length() > 0) {
                            current->data.push_back(buffer);
                            buffer = "";
                        }
                        XmlElem* n = new XmlElem();
                        n->parent = current;
                        current->child.push_back(n);
                        current = n;
                        read_mode = READ_NAME;
                    } else if(buffer.length() > 0 ||
                        (line[i] != ' ' && line[i] != '\t' && line[i] != '\n'))
                            buffer += line[i];
                    break;
                case READ_NAME:
                    switch(line[i]) {
                        case '/':
                            if(buffer.length() <= 0)
                                read_mode = READ_CLOSE;
                            break;
                        case '=':
                            current->fieldName.push_back(buffer);
                            if(buffer.length() > 0) {
                                buffer = "";
                            } else {
                                log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)+"): "
                                    +"unexpected sign '='.");
                            }
                            read_mode = READ_VALUE;
                            break;
                        case '>':
                            if(buffer.length() <= 0 && current->tag.size() <= 0) {
                                log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)+"): "
                                    +"element without name.");
                            }
                            current->tag.push_back(buffer);
                            buffer = "";
                            read_mode = READ_NONE;
                            break;
                        case ' ':
                        case '\t':
                        case '\n':
                            if(buffer.length() > 0) {
                                read_mode = READ_WAIT_VALUE;
                            }
                            break;
                        default:
                            buffer += line[i];
                            break;
                    }
                    break;
                case READ_WAIT_VALUE:
                    switch(line[i]) {
                        case '=':
                            current->fieldName.push_back(buffer);
                            buffer = "";
                            read_mode = READ_VALUE;
                            break;
                        case '>':
                            if(current->tag.size() <= 0) {
                                log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)+"): "
                                    +"element without name.");
                            }
                            current->tag.push_back(buffer);
                            buffer = "";
                            read_mode = READ_NONE;
                            break;
                        case ' ':
                        case '\t':
                        case '\n':
                            break;
                        default:
                            current->tag.push_back(buffer);
                            buffer = "";
                            buffer += line[i];
                            read_mode = READ_NAME;
                            break;
                    }
                    break;
                case READ_VALUE:
                    switch(line[i]) {
                        case '=':
                            log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)+"): "
                                +"unexpected sign '='.");
                            break;
                        case '>':
                            if(current->tag.size() <= 0) {
                                log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)+"): "
                                    +"element without name.");
                            }
                            current->fieldData.push_back(buffer);
                            buffer = "";
                            read_mode = READ_NONE;
                            break;
                        case ' ':
                        case '\t':
                        case '\n':
                            if(buffer.length() > 0) {
                                current->fieldData.push_back(buffer);
                                buffer = "";
                                read_mode = READ_NAME;
                            }
                            break;
                        default:
                            buffer += line[i];
                            break;
                    }
                    break;
                case READ_CLOSE:
                    if(line[i] != ' ' && line[i] != '\t' && line[i] != '\n'){
                        if(line[i] == '>') {
                            if(current->parent->tag[0] != buffer) {
                                log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)+"): "
                                    +"incorrect closing caret (comparing names "
                                    +current->parent->tag[0]+" and "+buffer+").");
                            }
                            buffer = "";
                            XmlElem* t = current;
                            current = current->parent;
                            current->child.pop_back();
                            current = current->parent;
                            delete t;
                            read_mode = READ_NONE;
                        } else buffer += line[i];
                    }
                    break;
                default:
                    log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)+"): "+"read char without read_mode.");
                    break;
            }
        }
        if(read_mode == READ_NONE && buffer.length() > 0) {
            current->data.push_back(buffer);
            buffer = "";
        }
    }
    in.close();
    log.msg("File closed successfully.");
    log.msg("Parsing took "+std::to_string(stopwatch.stop_watch()/1000000)+" ms.");
    return origin;
}
                /*
                case READ_NONE: // previous char read isn't from any < > closure
                    if(buffer.length() <= 0 &&
                        (line[i] == '\t' || line[i] == ' ' || line[i] == '\n')) break;
                        // Don't save any tabs, spaces or line brakes before line contains anything
                    if(line[i] == '<') {
                        if(buffer.length() > 0) {
                            current->data.push_back(buffer);
                            buffer = "";
                        }
                        read_mode = READ_INSIDE;
                        XmlElem* n = new XmlElem();
                        n->parent = current;
                        current->child.push_back(n);
                        current = n;
                    } else buffer += line[i];
                    break;
                case READ_INSIDE: // previous char read is from < > closure but not met any name
                    switch(line[i]) {
                        case '/':
                            read_mode = READ_CLOSE;
                            break;
                        case '>':
                            read_mode = READ_NONE;
                            break;
                        case '=':
                            read_mode = READ_VALUE;
                            break;
                        case ' ':
                        case '\t':
                        case '\n':
                            break;
                        default:
                            buffer += line[i];
                            read_mode = READ_NAME;
                            break;
                    }
                    break;
                case READ_NAME: // previous char read was from name (of elem or field of elem)
                    switch(line[i]) {
                        case '>': // met ending of closure
                            if(buffer.length() > 0) {
                                if(current->name.length() > 0) {
                                    log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)
                                        +"): renamed XmlElem.");
                                }
                                current->name = buffer;
                                buffer = "";
                            }
                            read_mode = READ_NONE;
                            break;
                        case ' ':
                        case '\t': // met ending of name
                        case '\n':
                            if(current->name.length() > 0) { // case name was already defined...
                                read_mode = READ_WAIT_VALUE; // we guess fieldData will be further.
                                // error messages about this will log when we will find no fieldData.
                            } else { // else we think it is the name
                                current->name = buffer;
                                buffer = "";
                                read_mode = READ_INSIDE;
                            }
                            break;
                        case '=': // met ending of fieldName
                            current->fieldName.push_back(buffer);
                            if(buffer.length() > 0) {
                                buffer = "";
                            } else {
                                log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)+
                                    "): empty field name found.");
                            }
                            read_mode = READ_VALUE; // and now waiting for input
                            break;
                            // so if a field should be declared in elem before its name,
                            // '=' should be placed RIGHT after the name.
                        default:
                            buffer += line[i];
                            break;
                    }
                    break;
                case READ_WAIT_VALUE: // previously we met name, but not '=', so we're waiting for 
                    switch(line[i]) {
                        case '>':
                            log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)
                                +"): expected a value.");
                            current->fieldData.push_back("");
                            read_mode = READ_NONE;
                            break;
                        case ' ':
                        case '\t':
                        case '\n':
                            break;
                        case '=':
                            current->fieldName.push_back(buffer);
                            buffer = "";
                            read_mode = READ_VALUE;
                            break;
                        default:
                            log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)
                                +"): double name in object.");
                            current->name = buffer;
                            buffer = "";
                            buffer += line[i];
                            read_mode = READ_NAME;
                            break;
                    }
                    break;
                case READ_VALUE:
                    switch(line[i]) {
                        case '>':
                            if(buffer.length() <= 0) {
                                log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)
                                    +"): empty field value found.");
                            }
                            current->fieldData.push_back(buffer);
                            buffer = "";
                            read_mode = READ_NONE;
                            break;
                        case ' ':
                        case '\t':
                        case '\n':
                            if(buffer.length() > 0) {
                                current->fieldData.push_back(buffer);
                                buffer = "";
                                read_mode = READ_INSIDE;
                            }
                            break;
                        default:
                            buffer += line[i];
                            break;
                    }
                    break;
                case READ_CLOSE:
                    if(line[i] != ' ' && line[i] != '\t' && line[i] != '\n'){
                        if(line[i] == '>') {
                            if(current->parent->name != buffer) {
                                log.msg("On ("+std::to_string(lineN)+", "+std::to_string(i)
                                    +"): incorrect closing caret (comparing names "
                                    +current->parent->name+" and "+buffer+").");
                            }
                            buffer = "";
                            XmlElem* t = current;
                            current = current->parent;
                            current->child.pop_back();
                            current = current->parent;
                            delete t;
                            read_mode = READ_NONE;
                        } else buffer += line[i];
                    }
                    break;
                default:
                    break;
            }
        }
        if(read_mode == READ_NONE && buffer.length() > 0) {
            current->data.push_back(buffer);
            buffer = "";
        }
    }
    in.close();
    log.msg("File closed successfully.");
    log.msg("Parsing took "+std::to_string(stopwatch.stop_watch()/1000000)+" ms.");
    return origin;
}
//*/