#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include "point3d.h"

Point3d::Point3d(){}

Point3d::Point3d(float x, float y, float z) : x(x),y(y),z(z){}

float Point3d::get_x(){return x;}
	
float Point3d::get_y(){return y;}

float Point3d::get_z(){return z;}

Point3d Point3d::cross(Point3d p1){
    return Point3d(p1.get_y()*z-p1.get_z()*y,p1.get_z()*x-p1.get_x()*z,p1.get_x()*y-p1.get_y()*x);
}

Point3d Point3d::minus(Point3d p1){
    return Point3d(x-p1.get_x(), y-p1.get_y(),z-p1.get_z());
}

void Point3d::normalize(){
    product(1/norm());
}

void Point3d::product(float value){
    x*=value;
    y*=value;
    z*=value;
}

float Point3d::norm(){
    return sqrt(dotproduct(*this));
}

float Point3d::dotproduct(Point3d p1){
    return x*p1.get_x()+y*p1.get_y()+z*p1.get_z();
}
