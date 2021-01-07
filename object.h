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
    
    void print( TGAImage &img,  const TGAColor &c);
    
};

#endif //__OBJECT_H__
