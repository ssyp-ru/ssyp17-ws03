#include <RealEngine/xml_parser.h>
#include <RealEngine/logger.h>
#include <RealEngine/stop_watch.h>
#include <sstream>
#include <fstream>

re::XmlElem::XmlElem() {
    parent = nullptr;
}


std::vector<re::XmlElem*> re::XmlElem::get_children(std::string name) {
    std::vector<re::XmlElem*> arr;
    StopWatch stopwatch;
    for(auto it = child.begin(); it != child.end(); it++) {
        if((*it)->name == name) arr.push_back(*it);
    }
    re::Log::msg(".getChildren on \""+this->name+"\" took "
        +std::to_string(stopwatch.stop_watch())+" ns.",
        re::Log::LEVEL::DEBUG);
    return arr;
}

void xml_elem_print_rec(re::XmlElem* elem, std::ofstream& out, int depth) {
    for(int i = 0; i < depth; i++) {
        out << '\t';
    }
    out << "<" << elem->name;
    for(auto& field : elem->field) {
        out << ' ' << field.first << "=\"" << field.second << '"';
    }
    bool has_child = elem->child.size() > 0;
    bool has_content = elem->content.length() > 0;
    if(!(has_child || has_content)) {
        out << '/' << '>' << std::endl;
        return;
    }
    out << '>';
    if(has_content) {
        std::string buffer = elem->content;
        size_t found = buffer.find('\n');
        has_content = found != buffer.npos;
        if(has_content) {
            do {
                found = buffer.find('\n');
                out << std::endl;
                for(int i = 0; i <= depth; i++) {
                    out << '\t';
                }
                out << buffer.substr(0,found);
                buffer = buffer.substr(found+1,buffer.npos);
                if(buffer.length() <= 0) break;
            } while(found != buffer.npos);
        } else {
            out << buffer;
        }
    }
    if(has_child||has_content) {
        out << std::endl;
    }
    for(re::XmlElem* e : elem->child) {
        xml_elem_print_rec(e, out, depth+1);
    }
    if(has_child||has_content) for(int i = 0; i < depth; i++) {
        out << '\t';
    }
    out << "</" << elem->name << '>';
    out << std::endl;
}

void re::XmlElem::print(std::string output_filename) {
    std::ofstream out(output_filename);
    if(!out.is_open()) {
        re::Log::msg("Could not open the file \""+output_filename+"\"!",
        re::Log::LEVEL::DEBUG);
        return;
    }
    StopWatch stopwatch;
    xml_elem_print_rec(this, out, 0);
    re::Log::msg("Printing \""+name+"\" took "+std::to_string(stopwatch.stop_watch()/1000000)+" ms.",
        re::Log::LEVEL::DEBUG);
}

re::XmlElem re::parse_xml(std::string input_filename) {
    std::ifstream in(input_filename);
    
    XmlElem origin;
    origin.name = input_filename;
    origin.parent = nullptr;
    if(!in.is_open()) {
        re::Log::msg("Could not open the file \""+input_filename+"\"!",
            re::Log::LEVEL::DEBUG);
        return origin;
    }
    re::Log::msg("File \""+input_filename+"\" found!",
        re::Log::LEVEL::DEBUG);
    StopWatch stopwatch; // recording the time for this cpp
    XmlElem* current = &origin; // current object reading now
    int lineN = 0; // Counter for lines used only for logging
    int is_comment = false; // support for commented xml 
    while(!in.eof()) {
        // read next line
        std::string buffer;
        getline(in, buffer);
        re::Log::msg("Read line #"+std::to_string(++lineN)+": \""+buffer+"\"",
        re::Log::LEVEL::DEBUG);

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

            // find any comments presented in line
            found = buffer.find("<!--");
            if(found != buffer.npos) {
                is_comment = true;
            } // if comment is multi-lined
            if(is_comment) {
                found_closure = buffer.find("-->");
                if(found_closure != buffer.npos) {
                    if(found != buffer.npos) {
                        buffer.erase(found, found_closure-found+3);
                    } else {
                        buffer.erase(0, found_closure+3);   
                    }
                    is_comment = false;
                } else {
                    if(found != buffer.npos) {
                        buffer.erase(found, found_closure);
                    } else {
                        break; 
                    }
                }
            }

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
                re::Log::msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                    +"Not found '>' on line.", re::Log::LEVEL::DEBUG);
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
                                re::Log::msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                    +"Can't exit main object.", re::Log::LEVEL::DEBUG);
                            }
                            current = current->parent;
                        } else {
                            re::Log::msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                +"Wrong name in closing brackets.", re::Log::LEVEL::DEBUG);
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
                            re::Log::msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                +"Couldn't find name.", re::Log::LEVEL::DEBUG);
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
                                    re::Log::msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                        +"A field without value.", re::Log::LEVEL::DEBUG);
                                    break;
                                }
                                if(f <= 0) {
                                    re::Log::msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                        +"Couldn't find field name.", re::Log::LEVEL::DEBUG);
                                    break;
                                }
                                name = str.substr(0,f);
                                value = str.substr(f+1,str.npos);
                                if(value.length() <= 0) {
                                    re::Log::msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                        +"Empty field value.", re::Log::LEVEL::DEBUG);
                                    break;
                                }
                                if(value[0] != '"') {
                                    re::Log::msg("Error on ("+std::to_string(lineN)+", \""+buffer+"\"): "
                                        +"Where is '\"'?", re::Log::LEVEL::DEBUG);
                                    break;
                                }
                                value.erase(0,1);
                                if(value[value.length() - 1] != '"') {
                                    is_string = true;
                                } else {
                                    value.erase(value.length() - 1, 1);
                                    elem->field[name] = value;
                                }
                            } else {
                                value += ' ';
                                value += str;
                                if(str.rfind('"') == str.length() - 1) {
                                    value.erase(value.length() - 1, 1);
                                    elem->field[name] = value;
                                    is_string = false;
                                }
                            }
                        }
                    break;
                }
            }
            // remove found closure
            buffer.erase(found,found_closure-found+1);
        }
    }
    in.close();
    re::Log::msg("File closed successfully.", re::Log::LEVEL::DEBUG);
    re::Log::msg("Parsing took "+std::to_string(stopwatch.stop_watch()/1000)+" micros.",
        re::Log::LEVEL::DEBUG);
    return origin;
}