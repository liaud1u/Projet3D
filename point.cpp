#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include "point.h"

Point::Point(){}

Point::Point(float x, float y) : x(x),y(y){}

float Point::get_x(){return x;}
	
float Point::get_y(){return y;}
