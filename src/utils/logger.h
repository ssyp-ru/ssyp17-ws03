#include <fstream>

namespace re {
    class Log {
        std::ofstream l1_;
    public:
        Log();
        std::ofstream& l1();
    };
}