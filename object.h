#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include "point2d.h"
#include "point3d.h"
#include "tgaimage.h"



class Object {
protected:
    const char* path;
    const char* texture_path;
    std::vector<Point3d> points;
    std::vector<std::vector<int>> faces;
    std::vector<std::vector<int>> textures_faces;
    std::vector<Point3d> texture_coord;
    TGAImage texture;
    
public:
    
    Object();
    
	Object(const char *filename, const char *texture_path);

    std::vector<std::vector<int>> get_faces(){return faces;}
    
    std::vector<std::vector<int>> get_texture_faces(){return textures_faces;}
    
    std::vector<Point3d> get_textures(){return texture_coord;}
    
    std::vector<Point3d> get_points(){return points;}
    
    TGAColor get_color(Point2d point, float light_intensity);
    
    TGAImage get_texture(){return texture;};
    
};

#endif //__OBJECT_H__
