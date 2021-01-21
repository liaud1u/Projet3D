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

void Point3d::normalize(){
    x= sqrt(x*x);
    y=sqrt(y*y);
    z=sqrt(z*z);
}
