#include "tgaimage.h"
#include "object.h"
#include "matrix.h"
#include "renderer.h"
#include <limits>
#include <iostream>
 
Point3d eye(2,0,3);

int main(int argc, char** argv) {
    
    TGAImage image(SIZE, SIZE, TGAImage::RGB);
    
    // Affichage de plusieurs angles de vue
    for(int i = -3 ; i< 4; i++){
        image.clear();
        eye.set_x(i);
        init(eye); 
        
        Object   eyes_in("./ressources/african_head/african_head_eye_inner.obj","./ressources/african_head/african_head_eye_inner_diffuse.tga","./ressources/african_head/african_head_eye_inner_nm.tga"); 
        Object   head("./ressources/african_head/african_head.obj","./ressources/african_head/african_head_diffuse.tga","./ressources/african_head/african_head_nm.tga");
        
        printTriangle(eyes_in,image,true,eye); 
        printTriangle(head,image,true,eye);
        
        image.flip_vertically();
        image.flip_horizontally();
        
        //Save image
        std::string str("head_output_triangle");
        str = str + std::to_string(i) ;
        str = str+ ".tga";
        image.write_tga_file(str.c_str()); 
        
        image.clear();
        init(eye);
        
        Object diablo("./ressources/diablo3_pose/diablo3_pose.obj","./ressources/diablo3_pose/diablo3_pose_diffuse.tga","./ressources/diablo3_pose/diablo3_pose_nm.tga");
        
        printTriangle(diablo,image,true,eye);
        
        image.flip_vertically();
        image.flip_horizontally();
        
        //Save image
        str = std::string("diablo_output_triangle");
        str = str + std::to_string(i) ;
        str = str+ ".tga";
        image.write_tga_file(str.c_str()); 
        
        image.clear();
        init(eye);
        
        Object eyes("./ressources/boggie/eyes.obj","./ressources/boggie/eyes_diffuse.tga","./ressources/boggie/eyes_nm_tangent.tga");
        Object headb("./ressources/boggie/head.obj","./ressources/boggie/head_diffuse.tga","./ressources/boggie/head_nm_tangent.tga");
        Object body("./ressources/boggie/body.obj","./ressources/boggie/body_diffuse.tga","./ressources/boggie/body_nm_tangent.tga");
        
        printTriangle(headb,image,true,eye);
        printTriangle(eyes,image,true,eye);
        printTriangle(body,image,true,eye);
        
        image.flip_vertically();
        image.flip_horizontally();
        
        //Save image
        str = std::string("boggie_output_triangle");
        str = str + std::to_string(i) ;
        str = str+ ".tga";
        image.write_tga_file(str.c_str()); 
    }
    return 0;
}
