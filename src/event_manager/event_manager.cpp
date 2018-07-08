#include "RealEngine/event.h"
#include "RealEngine/math.h"
#include "RealEngine/logger.h"

#include "event_manager.h"

#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

namespace re {

EventManager event_manager;
Subscriber::Subscriber(EventSubscriber * feature_sub){
    sub = feature_sub;
    this->recive_all_ = false;
}

void Subscriber::subscride_to_all(){
    this->recive_all_ = true;
}

void Subscriber::add_event_type(int category, int type){
    for(size_t i = 0; i < category_list_.size(); i++){
        if(category_list_[i] == category)
            break;
    }
    category_and_type_list_.push_back(re::Point2f(category, type));
}

void Subscriber::add_event_category(int category){
    for(size_t i = 0; i < category_and_type_list_.size(); i++){
        if(category_and_type_list_[i].x == category) 
                category_and_type_list_.erase(category_and_type_list_.begin() + i);
    }
    category_list_.push_back(category);
}

void Subscriber::delete_event_type(int category, int type){
    for(size_t i = 0; i < category_and_type_list_.size(); i++){
        if((category_and_type_list_[i].x == category) && (category_and_type_list_[i].y == type)){
            category_and_type_list_.erase(category_and_type_list_.begin() + i);
            break;
        }
    }
}

void Subscriber::delete_event_category(int category){
    for(size_t i = 0; i < category_list_.size(); i++){
        if(category_list_[i] == category) {
            category_list_.erase(category_list_.begin() + i);
            break;
        }
    }
}

int Subscriber::get_type_size(){
    return category_and_type_list_.size();
}

int Subscriber::get_category_size(){
    return category_list_.size();
}

bool Subscriber::is_subscribed_category(int category){
    if( this->recive_all_ ){
        return true;
    }
    for(size_t i = 0; i < category_list_.size(); i++){
        if(category_list_[i] == category)
            return true;
    }
    return false;
}

bool Subscriber::is_subscribed_type(int category, int type){
    if( this->recive_all_ ){
        return true;
    }
    for(size_t i = 0; i < category_and_type_list_.size(); i++){
        if((category_and_type_list_[i].x == category) && (category_and_type_list_[i].y == type))
            return true;
    }
    return false;
}



void EventManager::send_events (std::shared_ptr<Event> event){
    std::string describe_sring = event->get_describe_string();
    if (!describe_sring.empty())
        Log::msg(describe_sring, event->get_log_level());
    size_t subscriber_count = subscriber_list.size();
    for(size_t i = 0; i < subscriber_count; i++){
        if(subscriber_list[i].is_subscribed_category(event->get_category()) ||
           subscriber_list[i].is_subscribed_type(event->get_category(), event->get_type())) {
            subscriber_list[i].sub->on_event(event);              
        }    
    }
}

void EventManager::add_subscriber_to_all( EventSubscriber * feature_subscriber ){
    Subscriber n_subscriber(feature_subscriber);
    n_subscriber.subscride_to_all();
    subscriber_list.push_back(n_subscriber);
}

void EventManager::add_subscriber_category (EventSubscriber * feature_subscriber, int category){
    int flag = 0;
    for(size_t i = 0; i < subscriber_list.size(); i++){
        if(subscriber_list[i].sub == feature_subscriber){
            subscriber_list[i].add_event_category(category);
            flag = 1;
        }
    }

    if(flag == 0){
        subscriber_list.push_back(Subscriber(feature_subscriber));
        subscriber_list.back().add_event_category(category);
    }
}

void EventManager::add_subscriber_type (EventSubscriber * feature_subscriber, int category, int type){
    int flag = 0;
    for(size_t i = 0; i < subscriber_list.size(); i++){
        if(subscriber_list[i].sub == feature_subscriber){
            subscriber_list[i].add_event_type(category, type);
            flag = 1;
        }
    }
    if(flag == 0){
        subscriber_list.push_back(Subscriber(feature_subscriber));
        subscriber_list.back().add_event_type(category, type);
    }
}

void EventManager::unsubscribe(EventSubscriber * subscriber){
    for(size_t i = 0; i < subscriber_list.size(); i++){
        if(subscriber_list[i].sub == subscriber){
            subscriber_list.erase(subscriber_list.begin() + i);
            break;
        }
    }
}

EventSubscriber::~ EventSubscriber(){
    event_manager.unsubscribe(this);
}


void subscribe_to_event_type(EventSubscriber * event_subscriber, int category, int type){
    event_manager.add_subscriber_type(event_subscriber, category, type);
}


void subscribe_to_event_category(EventSubscriber * event_subscriber, int category){
    event_manager.add_subscriber_category(event_subscriber, category);
}


void publish_event(std::shared_ptr<Event> set_event){
    event_manager.send_events(set_event);
}


void subscribe_to_all(EventSubscriber * event_subscriber){
    event_manager.add_subscriber_to_all(event_subscriber);
}


}          // namespace re