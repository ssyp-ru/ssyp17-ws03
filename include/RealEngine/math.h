#pragma once

namespace re{
    class Point2f{
    public:
        float x, y;
        Point2f(float x, float y);
        
        //distance from (0,0) to the point
        float length0();
        //distance from one point to another
        float length(Point2f right);

        //operators:
        Point2f operator+(Point2f right);
        Point2f operator-(Point2f right);
        Point2f operator*(float alpha);
        Point2f operator/(float alpha);
        float operator*(Point2f right);
        bool operator==(Point2f right);
        bool operator!=(Point2f right);

        //reduces length to 1
        void normalize();
        //makes Point2f with length 1
        Point2f normalizedp();

        //true if two straights are parallel and false if they're not
        bool parallel(Point2f first, Point2f second, Point2f third, Point2f fourth); 

        //true if point is existsing and false if it's not
        bool isValid();  

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
}