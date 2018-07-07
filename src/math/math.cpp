#include <cmath>
#include <iostream>
#include <limits>

#include "RealEngine/math.h"

const float precision = 0.0000001;

namespace re{

Point2f::Point2f(){
    this->x = 0;
    this->y = 0;
}

Point2f::Point2f(float x, float y){
    this->x = x;
    this->y = y;
}

float Point2f::length(){  //distance from (0, 0) to point
    return sqrt(x*x + y*y);
}

float Point2f::distance_to(Point2f right){  //distance from one point to another
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

void Point2f::operator+=(Point2f right){
    x += right.x;
    y += right.y;    
}

void Point2f::operator-=(Point2f right){
    x -= right.x;
    y -= right.y;
}

void Point2f::operator*=(float alpha){
    x *= alpha;
    y *= alpha;
}

void Point2f::operator/=(float alpha){
    x /= alpha;
    y /= alpha;
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
    float len = length();
    x = x / len;
    y = y / len;
}

bool Point2f::isParallel(Point2f first, Point2f second, Point2f third, Point2f fourth){
    float a1 = first.equation_a(second);
    float a2 = third.equation_a(fourth);
    float b1 = first.equation_b(second);
    float b2 = third.equation_b(fourth);
    return (a1 * b2 - a2 * b1 == 0);
}

bool Point2f::isValid(){
    return (x != std::numeric_limits<float>::infinity() && y != std::numeric_limits<float>::infinity());
}

bool Point2f::isNull(){
    return (x == 0 && y == 0);
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

Point2f Point2f::Normalized()
{
	return Point2f(x / length(), y / length());
}
Point2f Point2f::projectOnVector(Point2f vec)
{
	// proj.x = ( dp / (b.x*b.x + b.y*b.y) ) * b.x;
	// proj.y = ( dp / (b.x*b.x + b.y*b.y) ) * b.y;
    if (vec.x == 0) return Point2f(0, y);
    if (vec.y == 0) return Point2f(x, 0);
	double scalarProduct = *this * vec;
	return Point2f((scalarProduct / (vec.x * vec.x + vec.y * vec.y)) * vec.x, 
		            (scalarProduct / (vec.x * vec.x + vec.y * vec.y)) * vec.y);
}
Point2f Point2f::getLeftNormal()
{
	return Point2f(y, -x);
}
Point2f Point2f::getRightNormal()
{
	return Point2f(-y, x);
}
double Point2f::ratio(Point2f vec)
{
	if (vec.x != 0)
		return x / vec.x;
	else
		if (vec.y != 0)
			return y / vec.y;
		else
			return 0;
}
Point2f Point2f::reflectFrom(Point2f mirror)
{
	return *this + (this->projectOnVector(mirror) - *this) * 2;
}
bool Point2f::isCollinearTo(Point2f vec)
{
    if (((vec.x == 0) && (x == 0)) || ((vec.y == 0) && (y == 0))) return true;
    return (vec.x / vec.y == x / y);
}
void Point2f::rotate(double radians)
{
	double newx, newy;
	newx = x * cos(radians) + y * sin(radians);
	newy = y * cos(radians) - x * sin(radians);
	x = newx;
	y = newy;
}
Point2f Point2f::rotated(double radians)
{
	return Point2f(x * cos(radians) + y * sin(radians), y * cos(radians) - x * sin(radians));
}
double Point2f::angleBetween(Point2f vec){
	double a = length(), b = vec.length(), c = (*this - vec).length();
	if ((a == 0) || (b == 0) || (c == 0)) return 0;
	return acos((a*a + b*b - c*c) / (2*a*b));
}
double Point2f::vecMul(Point2f vec)
{
    return x * vec.y - y * vec.x;
}


} // namespace re 
