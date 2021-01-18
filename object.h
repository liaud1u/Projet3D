#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include "point.h"



class Object {
protected:
    const char* path;
    std::vector<Point> points;
    std::vector<std::vector<int>> faces;
    
public:
    
    Object();
    
	Object(const char *filename);

    std::vector<std::vector<int>> get_faces(){return faces;}
    
    std::vector<Point> get_points(){return points;}
    
};

#endif //__OBJECT_H__
