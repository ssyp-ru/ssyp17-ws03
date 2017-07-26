#include "xml_parser.h"
#include "../utils/logger.h"
#include "../utils/stop_watch.h"
#include <sstream>
#include <fstream>

re::Log log("logs/xml_parser.txt");

re::XmlElem::XmlElem() {
    parent = nullptr;
}


std::vector<re::XmlElem*> re::XmlElem::get_children(std::string name) {
    std::vector<re::XmlElem*> arr;
    StopWatch stopwatch;
    for(auto it = child.begin(); it != child.end(); it++) {
        if((*it)->name == name) arr.push_back(*it);
    }
    log.msg(".getChildren on \""+this->name+"\" took "
        +std::to_string(stopwatch.stop_watch())+" ns.");
    return arr;
}

void xml_elem_print_rec(re::XmlElem* elem, std::ofstream& out, int depth) {
    for(int i = 0; i < depth; i++) {
        out << '\t';
    }
    out << "<" << elem->name;
    for(auto& field : elem->field) {
        out << ' ' << field.first << '=' << field.second;
    }
    bool has_child = elem->child.size() > 0;
    bool has_content = elem->content.length() > 0;
    bool is_closed = !(has_child || has_content);
    if(is_closed) {
        out << '/';
    }
    out << '>';
    if(has_child) {
        out << std::endl;
    }
    if(has_content) {
        if(has_child) {
            std::string buffer = elem->content;
            size_t found;
            do {
                found = buffer.find('\n');
                for(int i = 0; i <= depth; i++) {
                    out << '\t';
                }
                out << buffer.substr(0,found) << std::endl;
                buffer = buffer.substr(found+1,buffer.npos);
                if(buffer.length() <= 0) break;
            } while(found != buffer.npos);
        } else {
            out << elem->content;
        }
    }
    for(re::XmlElem* e : elem->child) {
        xml_elem_print_rec(e, out, depth+1);
    }
    if(!is_closed) {
        if(has_child) for(int i = 0; i < depth; i++) {
            out << '\t';
        }
        out << "</" << elem->name << '>';
    }
    out << std::endl;
}

void re::XmlElem::print(std::string output_filename) {
    std::ofstream out(output_filename);
    if(!out.is_open()) {
        log.msg("Could not open the file \""+output_filename+"\"!");
        return;
    }
    StopWatch stopwatch;
    xml_elem_print_rec(this, out, 0);
    log.msg("Printing \""+name+"\" took "+std::to_string(stopwatch.stop_watch()/1000000)+" ms.");
}

re::XmlElem re::parse_xml(std::string input_filename) {
    std::ifstream in(input_filename);
    
    XmlElem origin;
    origin.name = input_filename;
    origin.parent = nullptr;
    if(!in.is_open()) {
        log.msg("Could not open the file \""+input_filename+"\"!");
        return origin;
    }
    log.msg("File \""+input_filename+"\" found!");
    StopWatch stopwatch; // recording the time for this cpp
    XmlElem* current = &origin; // current object reading now
    int lineN = 0; // Counter for lines used only for logging
    int is_comment = false; // support for commented xml 
    while(!in.eof()) {
        // read next line
        std::string buffer;
        getline(in, buffer);
        log.msg("Read line #"+std::to_string(++lineN)+": \""+buffer+"\"");

        // remove any spaces before and after content of buffer
        size_t found = buffer.find_first_not_of(" \t");
        size_t found_closure;        
        if(found != buffer.npos) {
            buffer = buffer.substr(found, buffer.npos);
        }
        found = buffer.find_last_not_of(" \t");
        if(found != buffer.npos) {
            buffer = buffer.substr(0, found+1);
        }

        while(buffer.length() > 0) {
            found = buffer.find("<!--");
            if(found != buffer.npos) {
                is_comment = true;
            }

            // find any comments presented in line
            // or 
            if(is_comment) {
                found_closure = buffer.find("-->");
                if(found_closure != buffer.npos) {
                    if(found != buffer.npos) {
                        buffer.erase(found, found_closure-found+3);
                    } else {
                        buffer.erase(0, found_closure+3);   
                    }
                    is_comment = false;
                }
            }
            if(is_comment) break;

            // find any object-related marks
            found = buffer.find("<");
            if(found > 0) {
                // add non-object part of the string to current XmlElem
                if(current->content.length() > 0) {
                    current->content += '\n';
                }
                current->content += buffer.substr(0,found);

                if(found == buffer.npos) { // if no objects left in string
                    break;
                } else { // else continue reading the string
                    buffer.erase(0,found);
                    found = 0;
                }
            }

            // check if object has closing caret
            found_closure = buffer.find(">");
            if(found_closure == buffer.npos) {
                log.msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                    +"Not found '>' on line.");
                break;
            } else {
                std::string closure = buffer.substr(1,found_closure-found-1);
                // case </NAME>
                // case <?...?>
                switch(closure[0]) {
                    case '/':
                        closure.erase(0,1);
                        if(closure == current->name) {
                            if(current->parent == nullptr) {
                                log.msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                    +"Can't exit main object.");
                            }
                            current = current->parent;
                        } else {
                            log.msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                +"Wrong name in closing brackets.");
                        }
                    break;
                    case '?':
                        closure.erase(0,1);
                        if(closure[closure.length()-1] == '?') break;
                    default:
                        XmlElem* elem = new XmlElem();
                        elem->parent = current;
                        
                        // case <.../>
                        bool is_closed = closure[closure.length()-1] == '/';
                        if(is_closed) {
                            closure.erase(closure.length()-1, 1);
                        }
                        
                        std::stringstream line_stream(closure);
                        std::string str;
                        line_stream >> str;
                        if(str.length() <= 0) {
                            log.msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                +"Couldn't find name.");
                            break;
                        }
                        // <NAME ...>
                        elem->name = str;
                        current->child.push_back(elem);
                        if(!is_closed) { // We don't need to enter in already closed object
                            current = elem;
                        }

                        // <... {name=value}>
                        std::string name;
                        std::string value;
                        bool is_string = false; // case name="value" ("s are saved)
                        while(!line_stream.eof()) {
                            line_stream >> str;
                            if(!is_string) {
                                size_t f = str.find("=");
                                if(f == str.npos) {
                                    log.msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                        +"A field without value.");
                                    break;
                                }
                                if(f <= 0) {
                                    log.msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                        +"Couldn't find field name.");
                                    break;
                                }
                                name = str.substr(0,f);
                                value = str.substr(f+1,str.npos);
                                if(value.length() <= 0) {
                                    log.msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                        +"Empty field value.");
                                    break;
                                }
                                if(value[0] == '"') {
                                    is_string = true;
                                } else {
                                    elem->field[name] = value;
                                }
                            } else {
                                value += ' ';
                                value += str;
                                if(str.find('"') == str.length() - 1) {
                                    elem->field[name] = value;
                                    is_string = false;
                                }
                            }
                        }
                    break;
                }
                if(closure[0] == '/') {
                // case <...>
                } else {}  // end case <...>
            } // end if correct closure
            // remove found closure
            buffer.erase(found,found_closure-found+1);
        }
    }
    in.close();
    log.msg("File closed successfully.");
    log.msg("Parsing took "+std::to_string(stopwatch.stop_watch()/1000)+" micros.");
    return origin;
}