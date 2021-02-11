#include "tgaimage.h"
#include "object.h"
#include "matrix.h"
#include "renderer.h"
#include <limits>
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);
 
Point3d light(0,0,-1); 
Point3d eye(-1,1,3);
  

int main(int argc, char** argv) {
	TGAImage image(SIZE, SIZE, TGAImage::RGB);
    
	image.flip_vertically(); // Origin at the left bottom corner
           
    //Object object("./ressources/african_head/african_head.obj","./ressources/african_head/african_head_diffuse.tga");
    Object object("./ressources/diablo3_pose/diablo3_pose.obj","./ressources/diablo3_pose/diablo3_pose_diffuse.tga");
    //Object object("./ressources/boggie/body.obj","./ressources/boggie/body_diffuse.tga");
    
    printPoint2d(object,image,white);
    image.flip_vertically();
    
    //Save image
	image.write_tga_file("output_point.tga");
    
    image.clear();
     
    printLine(object,image,white);
    image.flip_vertically();
    
    //Save image
	image.write_tga_file("output_line.tga");
    
    
    image.clear();
    printTriangle(object,image,true,eye, light);
    image.flip_vertically();
     
    //Save image
	image.write_tga_file("output_triangle.tga"); 
     
    
	return 0;
}
