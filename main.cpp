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
  
Point3d eye(-1,1,3);
  

int main(int argc, char** argv) {
	TGAImage image(SIZE, SIZE, TGAImage::RGB);
    
	image.flip_vertically(); // Origin at the left bottom corner
    init();
     
    Object   eyes_in("./ressources/african_head/african_head_eye_inner.obj","./ressources/african_head/african_head_eye_inner_diffuse.tga");
    Object   eyes_out("./ressources/african_head/african_head_eye_outer.obj","./ressources/african_head/african_head_eye_outer_diffuse.tga");
    Object   head("./ressources/african_head/african_head.obj","./ressources/african_head/african_head_diffuse.tga");
    
    printTriangle(eyes_in,image,true,eye);
    //printTriangle(eyes_out,image,true,eye);
    printTriangle(head,image,true,eye);
    image.flip_vertically();
     
    //Save image
	image.write_tga_file("head_output_triangle.tga"); 
    
    image.clear();
    init();
    
    
    Object diablo("./ressources/diablo3_pose/diablo3_pose.obj","./ressources/diablo3_pose/diablo3_pose_diffuse.tga");
    
    printTriangle(diablo,image,true,eye);
    
    image.flip_vertically();
     
    //Save image
	image.write_tga_file("diablo_output_triangle.tga"); 
    
    image.clear();
    init();
    
    Object eyes("./ressources/boggie/eyes.obj","./ressources/boggie/eyes_diffuse.tga");
    Object headb("./ressources/boggie/head.obj","./ressources/boggie/head_diffuse.tga");
    Object body("./ressources/boggie/body.obj","./ressources/boggie/body_diffuse.tga");
            
    printTriangle(headb,image,true,eye);
    printTriangle(eyes,image,true,eye);
    printTriangle(body,image,true,eye);
    
    image.flip_vertically();
     
    //Save image
	image.write_tga_file("boggie_output_triangle.tga"); 
     
    
	return 0;
}
