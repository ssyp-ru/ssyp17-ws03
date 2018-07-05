#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

int main()
{
    int n, startx, starty,  endx, endy, indexx, indexy;
    cin >> n >> startx >> starty >>endx >> endy;
    pair<int,int> parent [100][100];
    int count [100][100];
    int map[100][100];
    vector<pair<int, int>> way;


    queue<pair<int,int>> wave;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
           cin >> map[i][j];
           count[i][j] = -1;
       }
    }
    wave.push(make_pair(startx, starty));
    indexx = startx;
    indexy = starty;
    count [indexx][indexy] = 0;
    while((indexx != endx) || (indexy != endy))
    {
        /*
        if((map[indexx - 1][indexy - 1] == 1) && (count [indexx - 1][indexy - 1] == -1))
        {
            wave.push(make_pair(indexx - 1, indexy - 1));
            count[indexx -1 ][indexy -1] = count[indexx][indexy] + 1;
            parent[indexx - 1][indexy -1] = make_pair(indexx, indexy);
        }

        if((map[indexx - 1][indexy + 1] == 1) && (count [indexx - 1][indexy + 1] == -1))
        {
            wave.push(make_pair(indexx - 1, indexy + 1));
            count[indexx - 1][indexy + 1] = count[indexx][indexy] + 1; 
            parent[indexx - 1][indexy + 1] = make_pair(indexx, indexy);
        }

        if((map[indexx + 1][indexy - 1] == 1) && (count [indexx + 1][indexy - 1] == -1))
        {
            wave.push(make_pair(indexx + 1, indexy - 1));
            count[indexx + 1][indexy - 1] = count[indexx][indexy] + 1; 
            parent[indexx + 1][indexy - 1] = make_pair(indexx, indexy);
        }
*/
        if((map[indexx + 1][indexy + 1] == 1) && (count [indexx + 1][indexy + 1] == -1))
        {
            wave.push(make_pair(indexx + 1, indexy + 1));
            count[indexx + 1][indexy + 1] = count[indexx][indexy] + 1; 
            parent[indexx + 1][indexy + 1] = make_pair(indexx, indexy);
        }

        if((map[indexx - 1][indexy] == 1) && (count [indexx - 1][indexy] == -1))
        {
            wave.push(make_pair(indexx - 1, indexy));
            count[indexx -1 ][indexy] = count[indexx][indexy] + 1;
            parent[indexx - 1][indexy] = make_pair(indexx, indexy);
        }

        if((map[indexx + 1][indexy] == 1) && (count [indexx + 1][indexy] == -1))
        {
            wave.push(make_pair(indexx + 1, indexy));
            count[indexx + 1][indexy] = count[indexx][indexy] + 1; 
            parent[indexx + 1][indexy] = make_pair(indexx, indexy);
        }

        if((map[indexx][indexy - 1] == 1) && (count [indexx][indexy - 1] == -1))
        {
            wave.push(make_pair(indexx, indexy - 1));
            count[indexx][indexy - 1] = count[indexx][indexy] + 1; 
            parent[indexx][indexy - 1] = make_pair(indexx, indexy);
        }

        if((map[indexx][indexy + 1] == 1) && (count [indexx][indexy + 1] == -1))
        {
            wave.push(make_pair(indexx, indexy + 1));
            count[indexx][indexy + 1] = count[indexx][indexy] + 1; 
            parent[indexx][indexy + 1] = make_pair(indexx, indexy);
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
        cout<< lastx << " " << lasty << endl;
        lastx = parent[lastx][lasty].first;
        lasty = parent[copy_lastx][lasty].second;
        copy_lastx = lastx;



    }





    /*
    way.push_back(make_pair(endx, endy));
    
    cout << "228758"
    for(int i = 0; i < count[endx][endy] - 1; i++){
        way.push_back(parent[lastx][lasty]);
        lastx = parent[lastx][lasty].first;
        lasty = parent[copy_lastx][lasty].first;
        copy_lastx = lastx;
    }
    way.push_back(make_pair(startx, starty));
    way.reverse(way.begin(), way.end());
    

    for(int i = 0; i < way.size(); i++)
        cout << way[i].first << " " << way[i].second << endl;

   /* for(int i = 0; i < n; i++)
        cout << parent[i] << " ";


    int* way = new int [count[end] + 1];
    way[count[end]] = end;

    for(int i = count[end] - 1; i >= 0; i--)
        way[i] = parent[way[i - 1]];


    for(int i = 0; i < count[end] + 1; i++)
        cout << way[i] << " ";

  */  



return 0;
}