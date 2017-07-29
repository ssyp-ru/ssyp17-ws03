#pragma once
#include <vector>
#include <functional>

namespace re {

template <typename E>
class Event {
public: 
    typedef std::function<void(E)> callback;
private: 
    std::vector<callback> listeners;
public:
    void add_listener(callback cb) {
        listeners.push_back(cb);
    }

    bool remove_listener(callback cb)  {
        size_t found = listeners.find(cb);
        if(found != listeners.end()) {
            listeners.erase(found);
            return true;
        }
        return false;
    }

    void call(E event) {
        for(callback& cb : listeners) {
            cb(event);
        }
    }
    
    void operator()(E event) {
        call(event);
    }
};

} // namespace re