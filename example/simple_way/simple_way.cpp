#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/camera.h>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <math.h>
#include <chrono>
#include "RealEngine/math.h"
#include "RealEngine/graphic/image.h"


class MainApp : public re::IBaseApp{
public:
    void setup() override {
        re::set_fps(60);
        int s = 5000;
        auto start_time = std::chrono::steady_clock::now();
        int s2 = s / n_;
        imgptr_ = std::make_shared<re::Image>( "blackwhite.png" );
        std::cin >> startx_ >> starty_ >>endx_ >> endy_;
        std::vector<int> B;
        B.reserve(n_);
        map_.reserve(n_);
        for(int i = 0; i < n_; i++){
            map_.push_back(B);
            for(int j = 0; j < n_; j++){
                sub_image_ = imgptr_->get_subimage(int((s2) * i), int((s2) * j), int(s2), int(s2));
                col_ = sub_image_->get_pix_color(int((s2/n_) * i + s2/(n_*2)) , int((s2/n_) * j + s2/(n_*2)));
                if(col_.r == 0){
                    map_[i].push_back(0);
                }else{
                    map_[i].push_back(1);
                }
                
            }
        }

        for(int i = 0; i < n_; i++){
            map_[i][0] = 0;
            map_[0][i] = 0;
            map_[i][n_-1] = 0;
            map_[n_-1][i] = 0;

        }

        way_.reserve(n_*n_);
        way_ = re::get_the_way(map_, n_, startx_, starty_, endx_, endy_, count_);
        auto finish_time = std::chrono::steady_clock::now();
        std::cout << "time = " << std::chrono::duration_cast
                                <std::chrono::milliseconds>(finish_time - start_time).count() << std::endl;
    }
    void update() override {
    }
    void display() override {
        re::background(re::WHITE);
    
        for(int i = 0; i < n_; i++){
            re::draw_line(re::Point2f(0, (size_ / n_)*i), re::Point2f(size_, (size_ / n_)*i), re::BLACK);
            re::draw_line(re::Point2f((size_ / n_)*i, 0), re::Point2f((size_ / n_)*i, size_), re::BLACK);
        } 

        for(int i = 0; i < n_; i++){
            for(int j = 0; j < n_; j++){
                if(count_[i][j] != -1)
                    re::draw_rectangle(j*(size_/n_), i*(size_/n_),  size_/n_, size_/n_, re::LIGHTGREEN);
                if(map_[i][j] == 0)
                    re::draw_rectangle(j*(size_/n_), i*(size_/n_),  size_/n_, size_/n_, re::BLACK);
            }
        }

        for(unsigned int i = 0; i < way_.size(); i++)
            re::draw_rectangle(way_[i].y*(size_/n_) + size_/(4*n_), way_[i].x*(size_/n_) + size_/(4*n_),
                                size_/(2*n_), size_/(2*n_), re::RED);
    }


private:
    re::ImagePtr imgptr_;
    re::ImagePtr sub_image_;
    re::Color col_;
    std::vector<std::vector<int>> map_;
    std::vector<std::vector<int>> count_;
    int n_ = 200;
    int startx_, starty_, endx_, endy_;
    int size_ = 600;
    std::vector<re::Point2f> way_;

};

int main(){
    int size = 600;
    re::setWindowName( "RealEngine" );
    re::runApp( size, size, std::make_shared<MainApp>() );
    return 0;
}
