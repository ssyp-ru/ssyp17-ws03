#include <cmath>
#include <iostream>
#include <limits>

#include "RealEngine/math.h"

const float precision = 0.0000001;

namespace re{


Point2f::Point2f(float x, float y){
    this->x = x;
    this->y = y;
}

float Point2f::length0(){  //distance from (0, 0) to point
    return sqrt(x*x + y*y);
}

float Point2f::length(Point2f right){  //distance from one point to another
    return sqrt((right.x - x)*(right.x - x) + (right.y - y)*(right.y - y));    
}

Point2f Point2f::operator+(Point2f right){  
    return Point2f(x + right.x, y + right.y);
}       

Point2f Point2f::operator-(Point2f right){
    return Point2f(x - right.x, y - right.y);
}

Point2f Point2f::operator*(float alpha){
    return Point2f(x * alpha, y * alpha);
}

Point2f Point2f::operator/(float alpha){
    return Point2f(x / alpha, y / alpha);
}
float Point2f::operator*(Point2f right){  //scalar
    return (x * right.x + y * right.y);
}
bool Point2f::operator==(Point2f right){
    return (abs(x - right.x) < precision && abs(y - right.y) < precision);
}
bool Point2f::operator!=(Point2f right){
    return (abs(x - right.x) > precision && abs(y - right.y) > precision);
}

void Point2f::normalize(){
    float len = length0();
    x = x / len;
    y = y / len;
}

Point2f Point2f::normalizedp(){
    float len = length0();
    return Point2f(x/len, y/len);
}

bool Point2f::parallel(Point2f first, Point2f second, Point2f third, Point2f fourth){
    float a1 = first.equation_a(second);
    float a2 = third.equation_a(fourth);
    float b1 = first.equation_b(second);
    float b2 = third.equation_b(fourth);
    return (a1 * b2 - a2 * b1 == 0);
}

bool Point2f::isValid(){
    return (x != std::numeric_limits<float>::infinity() && y != std::numeric_limits<float>::infinity());
}

//straight equation
float Point2f::equation_a(Point2f another){
    return (another.y - y);
}
float Point2f::equation_b(Point2f another){
    return (x - another.x);
}
float Point2f::equation_c(Point2f another){
    return (x * (y - another.y) + y * (another.x - x));
}
float coordinate_y(Point2f first, Point2f second, Point2f third, Point2f fourth){
    float a1 = first.equation_a(second);
    float a2 = third.equation_a(fourth);
    float b1 = first.equation_b(second);
    float b2 = third.equation_b(fourth);
    float c1 = first.equation_c(second);
    float c2 = third.equation_c(fourth);
    return ((a1*c2 - a2*c1) / (a2*b1 - a1*b2));
}
float coordinate_x(Point2f first, Point2f second, Point2f third, Point2f fourth){
    float a1 = first.equation_a(second);
    float b1 = first.equation_b(second);
    float c1 = first.equation_c(second);
    return (-((coordinate_y(first, second, third, fourth)*b1 + c1) / a1));        
}
Point2f intersectionofstraights(Point2f first, Point2f second, Point2f third, Point2f fourth){
    return Point2f(coordinate_x(first, second, third, fourth), coordinate_y(first, second, third, fourth));
}


} // namespace re 
