#include "tgaimage.h"
#include "object.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);


void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
    // If line is steep, we transpose the image
    bool reverse = (std::abs(x1-x0)<std::abs(y1-y0));
    
    //Swap x0 and y0, x1 and y1 
    if(reverse){
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    
    //We need the smaller x as x0 in order to print the line 
    if(x0>x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    
    //Travel all the x
    for(int x = x0; x<x1; x++){
        float t = (x-x0)/(float)(x1-x0);
    
        int y = y0*(1-t)+y1*t+0.5;
        
        if(reverse)
            image.set(y,x,color);
        else
            image.set(x,y,color);
    }
}


void printLine(Object &obj, TGAImage &img, const TGAColor &c){
    
    std::vector<Point> points = obj.get_points();
    std::vector<std::vector<int>> faces = obj.get_faces();
    
    for(std::vector<int> triangle : faces){
        for(int cpt = 0; cpt<3; cpt++){
            line((int)(SIZE/2+SIZE/2*points.at(triangle[cpt]).get_x()),(int)(SIZE/2+SIZE/2*-points.at(triangle[cpt]).get_y()),(int)(SIZE/2+SIZE/2*points.at(triangle[(cpt+1)%3]).get_x()),(int)(SIZE/2+SIZE/2*-points.at(triangle[(cpt+1)%3]).get_y()), img, c);
        }
    } 
}


int main(int argc, char** argv) {
	TGAImage image(SIZE, SIZE, TGAImage::RGB);
    
	image.flip_vertically(); // Origin at the left bottom corner
    
    //Test for line in each direction
    line(50,50,75,75,image,white);
    line(50,50,25,25,image,red);
    line(50,50,75,25,image,green);
    line(50,50,25,75,image,blue);
    
    //Save image
	image.write_tga_file("output.tga");
    
    image.clear();
    
    Object object("./ressources/african_head.obj");
    object.printPoint(image,white);
    
    //Save image
	image.write_tga_file("output_point.tga");
    
    
    image.clear();
     
    printLine(object,image,white);
    
    //Save image
	image.write_tga_file("output_line.tga");
    
    
	return 0;
}


