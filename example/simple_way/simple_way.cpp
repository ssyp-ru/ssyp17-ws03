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
#include "RealEngine/math.h"

int sqr(int a){
    return a*a;
}


 void add_pair(re::Point2f A, std::vector<re::Point2f> &wave,
  re::Point2f start , re::Point2f end){
        int n = wave.size();
        for(int i = 0; i < n; i++){
            if(sqrt(sqr(A.x - start.x) + sqr(A.y - start.y)) + 
            sqrt(sqr(A.x - end.x) + sqr(A.y - end.y))  <     
            sqrt(sqr(wave[i].x - start.x) + sqr(wave[i].y - start.y)) + 
            sqrt(sqr(wave[i].x - end.x) + sqr(wave[i].y - end.y))){
                wave.insert(wave.begin() + i, A);
                return;
            }
        }
        wave.push_back(A);
 }


class MainApp : public re::IBaseApp{
public:
   

    void setup() override {
        re::set_fps(60);

        next_step.push_back(re::Point2f(1, 0));
        next_step.push_back(re::Point2f(-1, 0));
        next_step.push_back(re::Point2f(0, 1));
        next_step.push_back(re::Point2f(0, -1));
        next_step.push_back(re::Point2f(1, 1));
        next_step.push_back(re::Point2f(-1, -1));
        next_step.push_back(re::Point2f(-1, 1));
        next_step.push_back(re::Point2f(1, -1));




        std::cin >> n >> startx >> starty >>endx >> endy;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
            std::cin >> map[i][j];
            count[i][j] = -1;
            }
        }
        wave.push_back(re::Point2f(startx, starty));
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
                add_pair(re::Point2f(indexx - 1, indexy - 1), wave,
                re::Point2f(startx, starty), re::Point2f(endx, endy));
                count[indexx -1 ][indexy -1] = count[indexx][indexy] + sqrt_2;
                parent[indexx - 1][indexy -1] = re::Point2f(indexx, indexy);
            }

            if((map[indexx + 1][indexy - 1] == 1) && ((count [indexx + 1][indexy - 1] == -1) ||
              ((count [indexx + 1][indexy - 1] != -1) &&
              (count [indexx + 1][indexy - 1] > count [indexx][indexy] + sqrt_2))))
            {
                add_pair(re::Point2f(indexx + 1, indexy - 1), wave,
                re::Point2f(startx, starty), re::Point2f(endx, endy));
                count[indexx + 1][indexy - 1] = count[indexx][indexy] + sqrt_2; 
                parent[indexx + 1][indexy - 1] = re::Point2f(indexx, indexy);
            }

            if((map[indexx - 1][indexy + 1] == 1) && ((count [indexx - 1][indexy + 1] == -1)||
              ((count [indexx - 1][indexy + 1] != -1) &&
              (count [indexx - 1][indexy + 1] > count [indexx][indexy] + sqrt_2))))
            {
                add_pair(re::Point2f(indexx - 1, indexy + 1), wave,
                re::Point2f(startx, starty), re::Point2f(endx, endy));
                count[indexx - 1][indexy + 1] = count[indexx][indexy] + sqrt_2; 
                parent[indexx - 1][indexy + 1] = re::Point2f(indexx, indexy);
            }

            if((map[indexx + 1][indexy + 1] == 1) && ((count [indexx + 1][indexy + 1] == -1)||
              ((count [indexx + 1][indexy + 1] != -1) &&
              (count [indexx + 1][indexy + 1] > count [indexx][indexy] + sqrt_2))))
            {
                add_pair(re::Point2f(indexx + 1, indexy + 1),  wave,
                re::Point2f(startx, starty), re::Point2f(endx, endy));
                count[indexx + 1][indexy + 1] = count[indexx][indexy] + sqrt_2; 
                parent[indexx + 1][indexy + 1] = re::Point2f(indexx, indexy);
            }



            std::cout << std::endl;

            for(int i = 0; i < n; i++){
                std::cout << std::endl;
                for(int j = 0; j < n; j++){
                    std::cout << count[i][j] << " ";
                }
            }


            if((map[indexx - 1][indexy] == 1) && ((count [indexx - 1][indexy] == -1)||
              ((count [indexx - 1][indexy] != -1) &&
              (count [indexx - 1][indexy] > count [indexx][indexy] + 1))))
            {
                add_pair(re::Point2f(indexx - 1, indexy), wave,
                re::Point2f(startx, starty), re::Point2f(endx, endy));
                count[indexx -1 ][indexy] = count[indexx][indexy] + 100;
                parent[indexx - 1][indexy] = re::Point2f(indexx, indexy);
            }

            if((map[indexx + 1][indexy] == 1) && ((count [indexx + 1][indexy] == -1)||
              ((count [indexx + 1][indexy] != -1) &&
              (count [indexx + 1][indexy] > count [indexx][indexy] + 100))))
            {
                add_pair(re::Point2f(indexx + 1, indexy), wave,
                re::Point2f(startx, starty), re::Point2f(endx, endy));
                count[indexx + 1][indexy] = count[indexx][indexy] + 100; 
                parent[indexx + 1][indexy] = re::Point2f(indexx, indexy);
            }

            if((map[indexx][indexy - 1] == 1) && ((count [indexx][indexy - 1] == -1)||
              ((count [indexx][indexy - 1] != -1) &&
              (count [indexx][indexy - 1] > count [indexx][indexy] + 100))))
            {
                add_pair(re::Point2f(indexx, indexy - 1), wave,
                re::Point2f(startx, starty), re::Point2f(endx, endy));
                count[indexx][indexy - 1] = count[indexx][indexy] + 100; 
                parent[indexx][indexy - 1] = re::Point2f(indexx, indexy);
            }

            if((map[indexx][indexy + 1] == 1) && ((count [indexx][indexy + 1] == -1)||
              ((count [indexx][indexy + 1] != -1) &&
              (count [indexx][indexy + 1] > count [indexx][indexy] + 100))))
            {
                add_pair(re::Point2f(indexx, indexy + 1), wave,
                re::Point2f(startx, starty), re::Point2f(endx, endy));
                count[indexx][indexy + 1] = count[indexx][indexy] + 100; 
                parent[indexx][indexy + 1] = re::Point2f(indexx, indexy);
            }
*/

            for( int i = 0; i < 8; i++){
                if((map[int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] == 1) && 
                  ((count [int(indexx + next_step[i].x)][int(indexy+ next_step[i].y)] == -1)||
                  ((count [int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] != -1) &&
                  (count [int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] > 
                  count [int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] + 100))))
                {
                add_pair(re::Point2f(indexx + next_step[i].x, indexy + next_step[i].y), wave,
                re::Point2f(startx, starty), re::Point2f(endx, endy));
                 
                if(i <= 3){
                    count[int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] = count[indexx][indexy] + 100;
                }else{  
                    count[int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] = count[indexx][indexy] + sqrt_2;
                }
                parent[int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] = re::Point2f(indexx, indexy);
                }


            }



            indexx = wave[0].x;
            indexy = wave[0].y;
            wave.erase(wave.begin());
        }


        int lastx = endx;
        int lasty = endy;
        int copy_lastx = lastx;


        while((lastx != startx) || (lasty != starty))
        {   
            way.push_back(re::Point2f(lastx,lasty));
            lastx = parent[lastx][lasty].x;
            lasty = parent[copy_lastx][lasty].y;
            copy_lastx = lastx;
        }
        way.push_back(re::Point2f(startx,starty));


    }

    void update() override {
    }
    void display() override {
        re::background(re::WHITE);
        for(int i = 0; i < (size / n); i++){
            re::draw_line(re::Point2f(0, (size / n)*i), re::Point2f(size, (size / n)*i), re::BLACK);
            re::draw_line(re::Point2f((size / n)*i, 0), re::Point2f((size / n)*i, size), re::BLACK);
        } 

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                re::draw_text(j*(size/n) + size/(4*n), i*(size/n) + size/(4*n), std::to_string(count[i][j]), re::BLACK);
                if(map[i][j] == 0)
                    re::draw_rectangle(j*(size/n), i*(size/n),  size/n, size/n, re::BLACK);
            }
        }


        for(unsigned int i = 0; i < way.size(); i++)
            re::draw_rectangle(way[i].y*(size/n) + size/(4*n), way[i].x*(size/n) + size/(4*n),
             size/(2*n), size/(2*n), re::LIGHTGREEN);

    }

   
private:
    int n, startx, starty,  endx, endy, indexx, indexy;
    int sqrt_2 = 141;
    int size = 600;
    re::Point2f parent [100][100];
    int count [100][100];
    int map[100][100];
    std::vector<re::Point2f> way;
    std::vector<re::Point2f> wave;
    std::vector<re::Point2f> next_step;

};

int main(){
    int size = 600;
    re::setWindowName( "RealEngine" );
    re::runApp( size, size, std::make_shared<MainApp>() );
    return 0;
}
