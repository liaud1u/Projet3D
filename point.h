#ifndef __POINT_H__
#define __POINT_H__


class Point {
protected:
    float x, y;
    
public:
    
    Point();
    
	Point(float x, float y);
    
	float get_x();
	
    float get_y();
    
};

#endif //__POINT_H__
