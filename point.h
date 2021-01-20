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
    
    void set_x(float x){this->x=x;}
    
    void set_y(float y){this->y=y;}
};

#endif //__POINT_H__
