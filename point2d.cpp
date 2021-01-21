#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include "point2d.h"

Point2d::Point2d(){}

Point2d::Point2d(float x, float y) : x(x),y(y){}

float Point2d::get_x(){return x;}
	
float Point2d::get_y(){return y;}
