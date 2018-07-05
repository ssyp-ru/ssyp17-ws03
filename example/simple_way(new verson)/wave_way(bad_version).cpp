#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/camera.h>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>


class MainApp : public re::IBaseApp{
public:
   
    void setup() override {
        re::set_fps(60);
        std::cin >> n >> startx >> starty >>endx >> endy;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
            std::cin >> map[i][j];
            count[i][j] = -1;
            }
        }
        wave.push(std::make_pair(startx, starty));
        indexx = startx;
        indexy = starty;
        count [indexx][indexy] = 0;
        while((indexx != endx) || (indexy != endy))
        {
/*
            if((map[indexx - 1][indexy - 1] == 1) && ((count [indexx - 1][indexy - 1] == -1) ||
              ((count [indexx - 1][indexy - 1] != -1) &&
              (count [indexx - 1][indexy - 1] > count [indexx][indexy] + sqrt_2))))
            {
                wave.push(std::make_pair(indexx - 1, indexy - 1));
                count[indexx -1 ][indexy -1] = count[indexx][indexy] + sqrt_2;
                parent[indexx - 1][indexy -1] = std::make_pair(indexx, indexy);
            }

            if((map[indexx + 1][indexy - 1] == 1) && ((count [indexx + 1][indexy - 1] == -1) ||
              ((count [indexx + 1][indexy - 1] != -1) &&
              (count [indexx + 1][indexy - 1] > count [indexx][indexy] + sqrt_2))))
            {
                wave.push(std::make_pair(indexx + 1, indexy - 1));
                count[indexx + 1][indexy - 1] = count[indexx][indexy] + sqrt_2; 
                parent[indexx + 1][indexy - 1] = std::make_pair(indexx, indexy);
            }

            if((map[indexx - 1][indexy + 1] == 1) && ((count [indexx - 1][indexy + 1] == -1)||
              ((count [indexx - 1][indexy + 1] != -1) &&
              (count [indexx - 1][indexy + 1] > count [indexx][indexy] + sqrt_2))))
            {
                wave.push(std::make_pair(indexx - 1, indexy + 1));
                count[indexx - 1][indexy + 1] = count[indexx][indexy] + sqrt_2; 
                parent[indexx - 1][indexy + 1] = std::make_pair(indexx, indexy);
            }

            if((map[indexx + 1][indexy + 1] == 1) && ((count [indexx + 1][indexy + 1] == -1)||
              ((count [indexx + 1][indexy + 1] != -1) &&
              (count [indexx + 1][indexy + 1] > count [indexx][indexy] + sqrt_2))))
            {
                wave.push(std::make_pair(indexx + 1, indexy + 1));
                count[indexx + 1][indexy + 1] = count[indexx][indexy] + sqrt_2; 
                parent[indexx + 1][indexy + 1] = std::make_pair(indexx, indexy);
            }


*/
            if((map[indexx - 1][indexy] == 1) && ((count [indexx - 1][indexy] == -1)||
              ((count [indexx - 1][indexy] != -1) &&
              (count [indexx - 1][indexy] > count [indexx][indexy] + 1))))
            {
                wave.push(std::make_pair(indexx - 1, indexy));
                count[indexx -1 ][indexy] = count[indexx][indexy] + 100;
                parent[indexx - 1][indexy] = std::make_pair(indexx, indexy);
            }

            if((map[indexx + 1][indexy] == 1) && ((count [indexx + 1][indexy] == -1)||
              ((count [indexx + 1][indexy] != -1) &&
              (count [indexx + 1][indexy] > count [indexx][indexy] + 100))))
            {
                wave.push(std::make_pair(indexx + 1, indexy));
                count[indexx + 1][indexy] = count[indexx][indexy] + 100; 
                parent[indexx + 1][indexy] = std::make_pair(indexx, indexy);
            }

            if((map[indexx][indexy - 1] == 1) && ((count [indexx][indexy - 1] == -1)||
              ((count [indexx][indexy - 1] != -1) &&
              (count [indexx][indexy - 1] > count [indexx][indexy] + 100))))
            {
                wave.push(std::make_pair(indexx, indexy - 1));
                count[indexx][indexy - 1] = count[indexx][indexy] + 100; 
                parent[indexx][indexy - 1] = std::make_pair(indexx, indexy);
            }

            if((map[indexx][indexy + 1] == 1) && ((count [indexx][indexy + 1] == -1)||
              ((count [indexx][indexy + 1] != -1) &&
              (count [indexx][indexy + 1] > count [indexx][indexy] + 100))))
            {
                wave.push(std::make_pair(indexx, indexy + 1));
                count[indexx][indexy + 1] = count[indexx][indexy] + 100; 
                parent[indexx][indexy + 1] = std::make_pair(indexx, indexy);
            }

            indexx = wave.front().first;
            indexy = wave.front().second;
            wave.pop();
        }


        int lastx = endx;
        int lasty = endy;
        int copy_lastx = lastx;


        while((lastx != startx) || (lasty != starty))
        {   
            way.push_back(std::make_pair(lastx,lasty));
            lastx = parent[lastx][lasty].first;
            lasty = parent[copy_lastx][lasty].second;
            copy_lastx = lastx;
        }
        way.push_back(std::make_pair(startx,starty));


    }

    void update() override {
    }
    void display() override {
        re::background(re::WHITE);
        for(int i = 0; i < (600 / n); i++){
            re::draw_line(re::Point2f(0, (600 / n)*i), re::Point2f(600, (600 / n)*i), re::BLACK);
            re::draw_line(re::Point2f((600 / n)*i, 0), re::Point2f((600 / n)*i, 600), re::BLACK);
        } 

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                re::draw_text(j*(600/n) + 600/(4*n), i*(600/n) + 600/(4*n), std::to_string(count[i][j]), re::BLACK);
                if(map[i][j] == 0)
                    re::draw_rectangle(j*(600/n), i*(600/n),  600/n, 600/n, re::BLACK);
            }
        }


        for(int i = 0; i < way.size(); i++)
            re::draw_rectangle(way[i].second*(600/n) + 600/(4*n), way[i].first*(600/n) + 600/(4*n),
             600/(2*n), 600/(2*n), re::LIGHTGREEN);

    }

   
private:
    int n, startx, starty,  endx, endy, indexx, indexy;
    int sqrt_2 = 141;
    std::pair<int,int> parent [100][100];
    int count [100][100];
    int map[100][100];
    std::vector<std::pair<int, int>> way;


    std::queue<std::pair<int,int>> wave;
};

int main(){
    re::setWindowName( "RealEngine" );
    re::runApp( 600, 600, std::make_shared<MainApp>() );
    return 0;
}
