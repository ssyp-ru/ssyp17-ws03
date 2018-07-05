#pragma once
#include <vector>
#include <functional>
#include <memory>

namespace re {

class Event{
public:
    Event(unsigned int event_category, unsigned int event_type) : event_category_(event_category), event_type_(event_type){}     

    std::vector<char> serialize()
    {
        return std::vector<char>();
    }

    void deserialize( std::vector<char> )
    {

    }

    unsigned int get_category()  
    { 
        return event_category_;
    }
    unsigned int get_type()      
    { 
        return event_type_; 
    }

private:
    const unsigned int event_category_;
    const unsigned int event_type_;
};
typedef std::shared_ptr<Event> EventPtr;


class EventSubscriber{
 public:
    virtual void on_event(std::shared_ptr<Event> event) = 0;

    virtual ~EventSubscriber();
};



void subscribe_to_event_type(EventSubscriber * event_sub, int category, int type);
void subscribe_to_event_category(EventSubscriber * event_sub, int category);
void subscribe_to_all(EventSubscriber * event_sub);
void publish_event(std::shared_ptr<Event> set_event);


} // namespace re