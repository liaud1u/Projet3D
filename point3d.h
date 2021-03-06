#ifndef __POINT3D_H__
#define __POINT3D_H__


class Point3d {
protected:
    float x, y, z;
    
public:
    
    Point3d();
    
	Point3d(float x, float y, float z);
    
	float get_x();
	
    float get_y();
    
    float get_z();
    
    void set_x(float x){this->x=x;}
    
    void set_y(float y){this->y=y;}
    
    void set_z(float z){this->z=z;}
    
    Point3d cross(Point3d p1);
    
    Point3d minus(Point3d p1);
    
    void product(float value);
    
    float get(int i);
    
    float dotproduct(Point3d p1);
    
    void normalize();
    
    float norm();
};

#endif //__POINT_H__
