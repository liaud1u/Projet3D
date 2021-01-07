#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include "point.h"

class Object {
protected:
    const char* path;
    std::vector<Point> points;
    
public:
    
    Object();
    
	Object(const char *filename);
    
};

#endif //__OBJECT_H__
