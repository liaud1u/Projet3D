#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <vector>
#include <limits>
#include <iostream>
#include "object.h"
#include "matrix.h"
#include "point2d.h"
#include "point3d.h"
#include "tgaimage.h"

 
#define SIZE 1024
#define DEPTH 255

struct IShader {
    virtual ~IShader(){};
    virtual Point3d vertex(Matrix mat, Matrix id, Matrix ViewPort, Matrix ModelView) = 0;
    virtual bool fragment(  std::vector<Point3d> points_vn, Point3d bc_screen, TGAColor &color, Point3d norm) = 0;
};

float max(float x, float y);

float min(float x, float y);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

void line(Point2d p0, Point2d p1, TGAImage &image, TGAColor color);


void printPoint2d(Object &obj, TGAImage &img, const TGAColor &c);


Point3d barycentric(std::vector<Point3d> &pts, Point2d P);

void printLine(Object &obj, TGAImage &img, const TGAColor &c);

void traceTriangle(std::vector<Point3d> points_tri, std::vector<Point3d> points_text,std::vector<Point3d>  points_vn,TGAImage &img,  double * zbuffer[], Object &obj, IShader &shader);


Matrix viewport(int x, int y, int w, int h);

Matrix lookat(Point3d eye, Point3d up);

void printTriangle(Object &obj, TGAImage &img, bool shading, Point3d eye);
 
void init(Point3d light_dir);

void rasterize(Point2d p0, Point2d p1, TGAImage &img,const TGAColor &c, int ybuffer[]);



#endif //__RENDERER_H__
