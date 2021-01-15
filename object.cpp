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

Object::Object(const char *filename) : path(filename){
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
                std::string to_delete,x,y;
                splited_string>> to_delete >> x >> y;
                Point p(std::stof(x),std::stof(y));
                points.push_back(p);
            }
            if(line[0]=='f' && line[1]==' '){ 
                std::istringstream splited_string(line.c_str());
                std::string to_delete, face;
                splited_string>> to_delete >> face;
                
                
                std::vector<int> triangle; 
                
                for(int c = 0; c<3; c++){ 
                
                    std::istringstream points(face.c_str());                
                    std::string token;
                    
                
                    std::getline(points, token, '/') ;
                    
                    triangle.push_back(std::stoi(token)-1);
                
                
                    splited_string >> face;
                    
                }
                
                    faces.push_back(triangle);
            }
        }
        
        in.close();
    }
}

void Object::printPoint( TGAImage &img, const TGAColor &c){
    for(Point p : points){
        
            img.set(SIZE/2+SIZE/2*p.get_x(),SIZE/2-p.get_y()*SIZE/2,c); 
    }
}



