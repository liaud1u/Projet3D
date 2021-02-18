#include <vector>
#include <cmath>
#include <iostream>
#include "matrix.h" 

Matrix::Matrix(int r, int c):m(std::vector<std::vector<float>>(r,std::vector<float>(c,(float)0))),r(r),c(c){}

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i=0; i<dimensions; i++) {
        for (int j=0; j<dimensions; j++) {
            E[i][j] = (i==j ? 1.f : 0.f);
        }
    }
    return E;
}

std::vector<float>& Matrix::operator[](const int i) { 
    return m[i];
}

Matrix Matrix::operator*( Matrix& a) { 
    Matrix result(r, a.get_c());
    for (int i=0; i<r; i++) {
        for (int j=0; j<a.get_c(); j++) {
            result[i][j] = 0.f;
            for (int k=0; k<c; k++) {
                result[i][j] += m[i][k]*a[k][j];
            }
        }
    }
    return result;
}


Matrix Matrix::fromP3D(Point3d p){
    Matrix mat(4,1);
    mat[0][0]=p.get_x();
    mat[1][0]=p.get_y();
    mat[2][0]=p.get_z();
    mat[3][0]=1;
    
    return mat;
} 

Point3d Matrix::toP3D(){
    Point3d res(0,0,0);
    if(r>=4 && c>=1){
        res.set_x(m[0][0]/m[3][0]);
        res.set_y(m[1][0]/m[3][0]);
        res.set_z(m[2][0]/m[3][0]);
    }
    
    return res;
}
