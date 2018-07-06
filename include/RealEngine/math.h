#pragma once
#include <vector>
namespace re{
    class Point2f{
    public:
        float x, y;
        Point2f();  
        Point2f(float x, float y);
        
        //distance from (0,0) to the point
        float length();
        //distance from one point to another
        float distance_to(Point2f right);

        //operators:
        Point2f operator+(Point2f right);
        Point2f operator-(Point2f right);
        Point2f operator*(float alpha);
        Point2f operator/(float alpha);
        void operator+=(Point2f right);
        void operator-=(Point2f right);
        void operator*=(float alpha);
        void operator/=(float alpha);
        float operator*(Point2f right); //scalar
        bool operator==(Point2f right);
        bool operator!=(Point2f right);

        //reduces length to 1
        void normalize();

        //true if two straights are parallel and false if they're not
        bool isParallel(Point2f first, Point2f second, Point2f third, Point2f fourth); 
        //true if point is existing and false if it's not
        bool isValid();  
        //true if point is (0,0) 
        bool isNull();

        //a, b and c of our straight equation 
        float equation_a(Point2f another);
        float equation_b(Point2f another);
        float equation_c(Point2f another);  
  
    };

    //coordinates y, x of intersection of straight lines (described with points 1,2 and points 3,4)
    float coordinate_y(Point2f first, Point2f second, Point2f third, Point2f fourth);
    float coordinate_x(Point2f first, Point2f second, Point2f third, Point2f fourth);
    //point of intersection of straight lines that are described with points 1,2 and points 3,4
    Point2f intersectionofstraights(Point2f first, Point2f second, Point2f third, Point2f fourth);



    // Search the way function

    void add_pair(re::Point2f addable_pair, std::vector<re::Point2f> &wave,
                  re::Point2f start , re::Point2f end, std::vector <std::vector<int>> &count );

    std::vector <re::Point2f> get_the_way(std::vector<std::vector<int>> map, int n, int startx,
    int starty, int endx, int endy, std::vector<std::vector<int>> &count);

}


