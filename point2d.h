#ifndef __POINT2D_H__
#define __POINT2D_H__


class Point2d {
protected:
    float x, y;
    
public:
    
    Point2d();
    
	Point2d(float x, float y);
    
	float get_x();
	
    float get_y();
    
    void set_x(float x){this->x=x;}
    
    void set_y(float y){this->y=y;}
};

#endif //__POINT_H__
