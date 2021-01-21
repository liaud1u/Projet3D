#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include "point2d.h"
#include "point3d.h"



class Object {
protected:
    const char* path;
    std::vector<Point3d> points;
    std::vector<std::vector<int>> faces;
    
public:
    
    Object();
    
	Object(const char *filename);

    std::vector<std::vector<int>> get_faces(){return faces;}
    
    std::vector<Point3d> get_points(){return points;}
    
};

#endif //__OBJECT_H__
