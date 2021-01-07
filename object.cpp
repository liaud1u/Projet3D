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
        }
        
        in.close();
    }
}

void Object::print( TGAImage &img, const TGAColor &c){
    for(Point p : points){
        
            img.set(500+500*p.get_x(),500-p.get_y()*500,c); 
    }
}
