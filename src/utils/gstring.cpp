#include <string>
#include <sstream>
#include <vector>
#include <iterator>

namespace re
{

template<typename Out>
void gsplit(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> str_split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    gsplit(s, delim, std::back_inserter(elems));
    return elems;
}

}