#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "point3d.h"

class Matrix {
protected:
    std::vector<std::vector<float>> m;
    int r, c;
    
public:
    
	Matrix(int r=4, int c=4);
     
    int get_r(){return r;}
    
    int get_c(){return c;}
      
     
    static Matrix identity(int dimensions);
    
    std::vector<float>& operator[](const int i);
    
    Matrix operator*( Matrix& a);

    static Matrix fromP3D(Point3d p); 
    
    Point3d toP3D(); 
      
};

#endif //__MATRIX_H__
