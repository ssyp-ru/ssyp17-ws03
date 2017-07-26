#include <iostream>
#include <limits>
#include <assert.h>
#include "RealEngine/math.h"

bool test1(){
    re::Point2f result;
    re::Point2f first(1,2); re::Point2f second(3,3);
    re::Point2f third(3,2); re::Point2f fourth(4,3);
    result = re::intersectionofstraights(first, second, third, fourth);
    re::Point2f answer(5,4);
    return (answer == result);
}

bool test2(){
    re::Point2f result;
    re::Point2f first(-2,1); re::Point2f second(0,-3);
    re::Point2f third(5,1); re::Point2f fourth(-1,-1);
    result = re::intersectionofstraights(first, second, third, fourth);
    re::Point2f answer(-1,-1);
    return (answer == result);
}

bool test3(){
    re::Point2f result;
    re::Point2f first(1,-1); re::Point2f second(5,3);
    re::Point2f third(5,0); re::Point2f fourth(1,4);
    result = re::intersectionofstraights(first, second, third, fourth);
    re::Point2f answer(3.5,1.5);
    return (answer == result);
}

bool test4(){
    re::Point2f result;
    re::Point2f answer(std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity());
    re::Point2f first(1,1); re::Point2f second(2,2);
    re::Point2f third(1,2); re::Point2f fourth(2,3);
    result = re::intersectionofstraights(first, second, third, fourth);
    return (answer == result);
}

bool test5(){
    re::Point2f result;
    re::Point2f first(1,1); re::Point2f second(2,2);
    re::Point2f third(-1,1); re::Point2f fourth(-2,2);
    result = re::intersectionofstraights(first, second, third, fourth);
    re::Point2f answer;
    return (answer == result);
}

bool test6(){
    re::Point2f result(0,0);
    re::Point2f first(-1,3); re::Point2f second(1,1);
    re::Point2f third(3,1); re::Point2f fourth(4,2);
    result = re::intersectionofstraights(first, second, third, fourth);
    re::Point2f answer(2,0);
    return (answer == result);
}

bool test7(){
    re::Point2f result;
    re::Point2f first(1,3); re::Point2f second(2,4);
    re::Point2f third(1,1); re::Point2f fourth(2,0);
    result = re::intersectionofstraights(first, second, third, fourth);
    re::Point2f answer(0,2);
    return (answer == result);
}

bool test8(){
    re::Point2f point(-1.5,3000);
    return (point.isValid() == true);
}

bool test9(){
    re::Point2f point(std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity());
    return (point.isValid() == false);
}

bool test10(){
    re::Point2f point;
    return (point.isNull() == true);
}

bool test11(){
    re::Point2f point(3,0);
    return (point.isNull() == false);
}

int main(){
    std::cout << "hooy" << std::endl;
    assert(test1()); //simple test with natural numbers       
    assert(test2()); //test also with negatives
    assert(test3()); //test with fractions
    assert(test4()); //test with parallels
    assert(test5()); //point of intersection is (0,0)
    assert(test6()); //point of intersection is on X straight
    assert(test7()); //point of intersection is on Y straight
    assert(test8()); //validation of valid point
    assert(test9()); //validation of invalid point
    assert(test10()); //isNull of null point
    assert(test11()); //isNull of not null point
    
    return 0;
}