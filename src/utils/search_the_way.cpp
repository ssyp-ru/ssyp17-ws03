#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include "RealEngine/math.h"


void re::add_pair(re::Point2f addable_pair, std::vector<re::Point2f> &wave,
  re::Point2f start , re::Point2f end, std::vector <std::vector<int>> &count ){
       int n = wave.size();
        for(int i = 0; i < n; i++){
            if(count[int(addable_pair.x)][int(addable_pair.y)] +       
               sqrt(pow(addable_pair.x - end.x, 2) + pow(addable_pair.y - end.y, 2)) <     
               count[int(wave[i].x)][int(wave[i].y)] + 
               sqrt(pow(wave[i].x - end.x, 2) + pow(wave[i].y - end.y, 2))){
                wave.insert(wave.begin() + i, addable_pair);
                return;
            }
        }
        wave.push_back(addable_pair);
}

std::vector <re::Point2f> re::a_star(std::vector<std::vector<int>> map, int n, int startx, int starty,
                                          int endx, int endy, std::vector<std::vector<int>> &count){
    int indexx, indexy;
    std::vector<std::vector<re::Point2f>> parent;
    std::vector<re::Point2f> way;
    std::vector<re::Point2f> wave;
    std::vector<re::Point2f> next_step;
    next_step.push_back(re::Point2f(1, 0));
    next_step.push_back(re::Point2f(-1, 0));
    next_step.push_back(re::Point2f(0, 1));
    next_step.push_back(re::Point2f(0, -1));
    std::vector<int> B;
    B.reserve(n);
    std::vector <re::Point2f> B1;
    B1.reserve(n);
    count.reserve(n);
    parent.reserve(n);
    for(int i = 0; i < n; i++){
        count.push_back(B);
        parent.push_back(B1);
        for(int j = 0; j < n; j++){
            count[i].push_back(-1);
            parent[i].push_back(re::Point2f(0,0));
        }
    }
    indexx = startx;
    indexy = starty;
    count [indexx][indexy] = 0;
    while((indexx != endx) || (indexy != endy)){  
        for( int i = 0; i < 4; i++){
            if((map[int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] == 0) &&  
                ((count [int(indexx + next_step[i].x)][int(indexy+ next_step[i].y)] == -1)||
                ((count [int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] != -1) &&
                (count [int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] > 
                count [int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] + 1)))){
            count[int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] = count[indexx][indexy] + 1;                          
            parent[int(indexx + next_step[i].x)][int(indexy + next_step[i].y)] = re::Point2f(indexx, indexy);
            re::add_pair(re::Point2f(indexx + next_step[i].x, indexy + next_step[i].y), wave,
            re::Point2f(startx, starty), re::Point2f(endx, endy), count);      
            }
        }
       // std::cout << indexx << " " << indexy << std::endl; 
        indexx = wave[0].x;
        indexy = wave[0].y;
        wave.erase(wave.begin());
    }
    int lastx = endx;
    int lasty = endy;
    int copy_lastx = lastx;
    while((lastx != startx) || (lasty != starty)){   
        way.push_back(re::Point2f(lastx, lasty));
        lastx = parent[lastx][lasty].x;
        lasty = parent[copy_lastx][lasty].y;
        copy_lastx = lastx;
    }
    reverse(way.begin(), way.end());
    return way;
    
}


