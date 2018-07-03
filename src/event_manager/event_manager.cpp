#include "RealEngine/event.h"
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include "RealEngine/math.h"
#include "event_manager.h"
namespace re {

       
    Subscriber::Subscriber(EventSubscriber * feature_sub ){
        sub = feature_sub;
    }


    void Subscriber::add_event_type(int category, int type)
    {
        int i;
        for(i = 0; i < category_list.size(); i++){
            if(category_list[i] == category)
                break;
        }
        category_and_type_list.push_back(re::Point2f(category, type));
    }


    void Subscriber::add_event_category(int category)
    {
        int i;
        for(i = 0; i < category_and_type_list.size(); i++){
            if(category_and_type_list[i].x == category) 
                    category_and_type_list.erase(category_and_type_list.begin() + i);
        }
        category_list.push_back(category);
    }


    void Subscriber::delete_event_type(int category, int type)
    {
        int i;
        for(i = 0; i < category_and_type_list.size(); i++)
        {
            if((category_and_type_list[i].x == category) && (category_and_type_list[i].y == type))
            {
                category_and_type_list.erase(category_and_type_list.begin() + i);
                break;
            }
        }
    }

    void Subscriber::delete_event_category(int category)
    {
        int i;
        for(i = 0; i < category_list.size(); i++)
        {
            if(category_list[i] == category) 
            {
                category_list.erase(category_list.begin() + i);
                break;
            }
        }
    }
    
    int Subscriber::get_type_size(){
        return category_and_type_list.size();
    }

        int Subscriber::get_category_size(){
        return category_list.size();
    }
    
    bool Subscriber::is_subscribed_category(int category)
    {
        int i;
        for(i = 0; i < category_list.size(); i++)
        {
            if(category_list[i] == category)
                return true;
        }
        return false;
    }

    bool Subscriber::is_subscribed_type(int category, int type)
    {
        int i;
        for(i = 0; i < category_and_type_list.size(); i++)
        {
            if(( category_and_type_list[i].x == category) && ( category_and_type_list[i].y == type))
                return true;
        }
        return false;
    }

   

    void EventManager::send_events (std::shared_ptr<Event> event)
    {
        int i;
        int j;
        for(i = 0; i < subscriber_list.size(); i++)
        {
            
                if(subscriber_list[i].is_subscribed_category(event->get_category()))
                    subscriber_list[i].sub->on_event(event);              

                if(subscriber_list[i].is_subscribed_type(event->get_category(), event->get_type()))
                    subscriber_list[i].sub->on_event(event);
                
        }
    }

    void EventManager::add_subscriber_type (EventSubscriber * feature_subscriber, int category, int type)
    {
        int i;
        int flag = 0;
        for(i = 0; i < subscriber_list.size(); i++)
        {
            if(subscriber_list[i].sub == feature_subscriber)
            {
                subscriber_list[i].add_event_type(category, type);
                flag = 1;
            }

        }

        if(flag == 0)
        {
        subscriber_list.push_back(Subscriber(feature_subscriber));
        subscriber_list.back().add_event_type(category, type);
        }
    }


    void EventManager::add_subscriber_category (EventSubscriber * feature_subscriber, int category)
    {
        int i;
        int flag = 0;
        for(i = 0; i < subscriber_list.size(); i++)
        {
            if(subscriber_list[i].sub == feature_subscriber)
            {
                subscriber_list[i].add_event_category(category);
                flag = 1;
            }

        }

        if(flag == 0)
        {
        subscriber_list.push_back(Subscriber(feature_subscriber));
        subscriber_list.back().add_event_category(category);
        }
    }

    void EventManager::unsubscribe(EventSubscriber * subscriber)
    {
        for(int i = 0; i < subscriber_list.size(); i++)
        {
            if(subscriber_list[i].sub == subscriber)
            {
                subscriber_list.erase(subscriber_list.begin() + i);
                break;
            }
        }
    }
    // typedef std::shared_ptr<EventManager> EventManagerPtr;
    
    EventManager event_manager;

    void subscribe_to_event_type(EventSubscriber * event_subscriber, int category, int type)
    {
        event_manager.add_subscriber_type(event_subscriber, category, type);
    }

     void subscribe_to_event_category(EventSubscriber * event_subscriber, int category)
    {
        event_manager.add_subscriber_category(event_subscriber,category);
    }

    void publish_event(std::shared_ptr<Event> set_event)
    {
        event_manager.send_events (set_event);
    }

    EventSubscriber::~ EventSubscriber()
    {
        event_manager.unsubscribe(this);


    }


}          // namespace re