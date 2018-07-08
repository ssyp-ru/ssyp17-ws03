#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/event.h>
#include <iostream>
#include <memory>

class Hero1 : public re::EventSubscriber
{
public:
    int hp;
    int damage;
    Hero1 * hero_for_list = this;
    Hero1(unsigned int x, unsigned int y){
        hp = x;
        damage = y;
    }
    void on_event(re::EventPtr event){
        std::cout << "Hero1::on_event " << event->get_category() << ":" << event->get_type() << "\n";
        hp += event->get_type();
    }

};
class Hero2 : public re::EventSubscriber
{
public:
    int hp;
    int damage;
    Hero2 * hero_for_list = this;
    Hero2(unsigned int x, unsigned int y){
        hp = x;
        damage = y;
    }
    void on_event(re::EventPtr event){
        std::cout << "Hero2::on_event " << event->get_category() << ":" << event->get_type() << "\n";
        hp += (event->get_type());
    }

};


class Heal : public re::Event
{
public:
    int heal;
    Heal(int x) : re::Event(4, 8){
        heal = x;
    }

    virtual std::string get_describe_string() { return ""; };
    virtual re::Log::LEVEL get_log_level() { return re::Log::LEVEL::DEBUG; }
};
class Heal2 : public re::Event
{
public:
    int heal;
    Heal2(int x) : re::Event(4, 6){
        heal = x;
    }

    virtual std::string get_describe_string() { return ""; };
    virtual re::Log::LEVEL get_log_level() { return re::Log::LEVEL::DEBUG; }
   
};



int main(){
    Hero1  hero1(10,10);
    Hero2  hero2(10,10);


    std::shared_ptr<Heal> morehp = std::make_shared<Heal>(5);
    std::shared_ptr<Heal2> morehp2 = std::make_shared<Heal2>(5);

    subscribe_to_event_type(&hero1, 4, 8);
    subscribe_to_event_category(&hero2, 4);
   
   // publish_event(morehp2);
    publish_event(morehp);

    std::cout << hero1.hp<< " " << hero2.hp <<std::endl;


    return 0;
}
