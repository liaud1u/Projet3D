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

Object::Object(const char *filename, const char *texture_path) : path(filename), texture_path(texture_path){ 

    texture.read_tga_file(texture_path);
    texture.flip_horizontally();  
    texture.flip_vertically();  
    
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
                Point3d p(std::stof(x),std::stof(y),std::stof(z));
                texture_coord.push_back(p);
            }
            if(line[0]=='f' && line[1]==' '){ 
                std::istringstream splited_string(line.c_str());
                std::string to_delete, face;
                splited_string>> to_delete >> face;
                
                
                std::vector<int> triangle; 
                std::vector<int> texture_triangle; 
                
                for(int c = 0; c<3; c++){ 
                
                    std::istringstream points(face.c_str());                
                    std::string token;
                    
                    
                    std::getline(points, token, '/') ;
                    triangle.push_back(std::stoi(token)-1);
                    std::getline(points, token, '/') ;  
                    texture_triangle.push_back(std::stoi(token)-1);
                    
                    
                    splited_string >> face;
                    
                }
                
                faces.push_back(triangle);
                textures_faces.push_back(texture_triangle);
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
    

