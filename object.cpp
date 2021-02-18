#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include <vector>
#include "tgaimage.h"
#include "object.h"

Object::Object(){}

Object::Object(const char *filename, const char *texture_path, const char *tan_path) : path(filename), texture_path(texture_path), tan_path(tan_path){ 

    texture.read_tga_file(texture_path); 
    texture.flip_vertically();  
    vn_tan.read_tga_file(tan_path); 
    vn_tan.flip_vertically();  
    
    std::ifstream in;
	in.open (filename, std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "L'objet n'existe pas " << filename << "\n";
		in.close();
	}else{
        std::cout << "Objet chargé avec succès \n";
        
        std::string line ="";
        
        
        while(std::getline(in,line)){
            if(line[0]=='v' && line[1]==' '){ 
                std::istringstream splited_string(line.c_str());
                std::string to_delete,x,y,z;
                splited_string>> to_delete >> x >> y >> z; 
                Point3d p(std::stof(x),std::stof(y),std::stof(z));
                points.push_back(p);
            }
            if(line[0]=='v' && line[1]=='t'){ 
                std::istringstream splited_string(line.c_str());
                std::string to_delete,x,y,z;
                splited_string>> to_delete >> x >> y>>z; 
                Point3d p(std::stof(x),std::stof(y),0);
                
                if(z!="")
                    p.set_z(std::stof(z));
                
                texture_coord.push_back(p);
            }
            
            if(line[0]=='v' && line[1]=='n'){ 
                std::istringstream splited_string(line.c_str());
                std::string to_delete,x,y,z;
                splited_string>> to_delete >> x >> y>>z; 
                Point3d p(std::stof(x),std::stof(y),0);
                
                if(z!="")
                    p.set_z(std::stof(z));
                
                vn_coord.push_back(p);
            }
            if(line[0]=='f' && line[1]==' '){ 
                std::istringstream splited_string(line.c_str());
                std::string to_delete, face;
                splited_string>> to_delete >> face;
                
                
                std::vector<int> triangle; 
                std::vector<int> texture_triangle; 
                std::vector<int> vn_triangle; 
                
                for(int c = 0; c<3; c++){ 
                
                    std::istringstream points(face.c_str());                
                    std::string token;
                    
                    
                    std::getline(points, token, '/') ;
                    triangle.push_back(std::stoi(token)-1);
                    std::getline(points, token, '/') ;  
                    texture_triangle.push_back(std::stoi(token)-1);
                    std::getline(points, token, '/') ;  
                    vn_triangle.push_back(std::stoi(token)-1);
                    
                    
                    splited_string >> face;
                    
                }
                
                faces.push_back(triangle);
                textures_faces.push_back(texture_triangle);
                vn_faces.push_back(vn_triangle);
            }
        }
        
         
        
        in.close();
    }
}


TGAColor Object::get_color(Point2d point, float light_intensity){
    TGAColor c = texture.get((point.get_x()*(texture.get_width())),(point.get_y()*(texture.get_height()))); 
    c.r=c.r*light_intensity;
    c.g=c.g*light_intensity;
    c.b=c.b*light_intensity;
    return c;
}
    
    
Point3d Object::get_uv(Point2d point ){; 
    TGAColor c = vn_tan.get((point.get_x()*(texture.get_width())),(point.get_y()*(texture.get_height()))); 
    float x =(float)c.r/255.f*2.f - 1.f; 
    float y =(float)c.g/255.f*2.f - 1.f; 
    float z =(float)c.b/255.f*2.f - 1.f; 
     
    Point3d res(x,y,z);
    res.normalize();
    
    return res;
}

