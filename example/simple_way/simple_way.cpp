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
        int s2 = s / n;
        imgptr = std::make_shared<re::Image>( "blackwhite.png" );
        std::cin >> startx >> starty >>endx >> endy;
        std::vector<int> B;
        B.reserve(n);
        map.reserve(n);
        for(int i = 0; i < n; i++){
            map.push_back(B);
            for(int j = 0; j < n; j++){
                sub_image = imgptr->get_subimage(int((s2) * i), int((s2) * j), int(s2), int(s2));
                col = sub_image->get_pix_color(int((s2/n) * i + s2/(n*2)) , int((s2/n) * j + s2/(n*2)));
                if(col.r == 0){
                    map[i].push_back(0);
                }else{
                    map[i].push_back(1);
                }
                
            }
        }

        for(int i = 0; i < n; i++){
            map[i][0] = 0;
            map[0][i] = 0;
            map[i][n-1] = 0;
            map[n-1][i] = 0;

        }

        way.reserve(n*n);
        way = re::get_the_way(map, n, startx, starty, endx, endy, count);
        auto finish_time = std::chrono::steady_clock::now();
        std::cout << "time = " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time).count() << "\n";
    }
    void update() override {
    }
    void display() override {
        re::background(re::WHITE);
    
        for(int i = 0; i < n; i++){
            re::draw_line(re::Point2f(0, (size / n)*i), re::Point2f(size, (size / n)*i), re::BLACK);
            re::draw_line(re::Point2f((size / n)*i, 0), re::Point2f((size / n)*i, size), re::BLACK);
        } 

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(count[i][j] != -1)
               re::draw_rectangle(j*(size/n), i*(size/n),  size/n, size/n, re::LIGHTGREEN);
                if(map[i][j] == 0)
                    re::draw_rectangle(j*(size/n), i*(size/n),  size/n, size/n, re::BLACK);
            }
        }

        for(unsigned int i = 0; i < way.size(); i++)
            re::draw_rectangle(way[i].y*(size/n) + size/(4*n), way[i].x*(size/n) + size/(4*n),
                                size/(2*n), size/(2*n), re::RED);

    }


private:
    re::ImagePtr imgptr;
    re::ImagePtr sub_image;
    re::Color col;
    std::vector<std::vector<int>> map;
    std::vector<std::vector<int>> count;
    int n = 150;
    int startx, starty, endx, endy;
    int size = 750;
    std::vector<re::Point2f> way;

};

int main(){
    int size = 750;
    re::setWindowName( "RealEngine" );
    re::runApp( size, size, std::make_shared<MainApp>() );
    return 0;
}
