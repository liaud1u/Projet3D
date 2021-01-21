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
