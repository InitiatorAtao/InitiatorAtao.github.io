#include "point.h"

static int allocate_num = 0;
static int destroy_num = 0;

Point::Point(){
    // std::cerr<<"normal creating data "<<this<<std::endl;
    allocate_num++;
}

Point::Point(int _x, int _y) : x(_x), y(_y){
    // std::cerr<<"xy creating data "<<this<<std::endl;
    allocate_num++;
}

Point::Point(const Point& other): x(other.x), y(other.y){
    // std::cerr<<"copy creating data "<<this<<std::endl;
    allocate_num++;
}

Point::~Point(){
    // std::cerr<<"destroying data "<<this<<std::endl;
    destroy_num++;
}

void Point::show_status(){
    std::cout << allocate_num << " " << destroy_num << std::endl;
}