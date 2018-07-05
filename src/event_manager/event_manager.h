#pragma once
#include "RealEngine/event.h"
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include "RealEngine/math.h"
namespace re {

class Subscriber
{
public:
    EventSubscriber * sub;
    Subscriber(EventSubscriber * feature_sub );
    void add_event_type(int category, int type);
    void add_event_category(int category);
    void subscride_to_all();
    void delete_event_type(int category, int type);
    void delete_event_category(int category);
    int get_type_size();
    int get_category_size();
    bool is_subscribed_category(int category);
    bool is_subscribed_type(int category, int type);
private:
    std::vector <re::Point2f> category_and_type_list;
    std::vector <int> category_list;
    bool recive_all;
};

class EventManager
{
public:
    void send_events (std::shared_ptr<Event> event);
    void add_subscriber_type (EventSubscriber * feature_subscriber, int category, int type);
    void add_subscriber_category (EventSubscriber * feature_subscriber, int category);
    void unsubscribe(EventSubscriber * subscriber);
private:
    std::vector <Subscriber> subscriber_list;    
};
} //namespace re