#include "tgaimage.h"
#include "object.h"
#include <iostream>


#define SIZE 2000

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

void line(Point p0, Point p1, TGAImage &image, TGAColor color){
line(p0.get_x(), p0.get_y(), p1.get_x(), p1.get_y(), image, color);
}


void printPoint(Object &obj, TGAImage &img, const TGAColor &c){
    for(Point p : obj.get_points()){
        img.set(SIZE/2+SIZE/2*p.get_x(),SIZE/2-p.get_y()*SIZE/2,c); 
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


void traceTriangle(std::vector<Point> points_tri, TGAImage &img, const TGAColor &c){
     //Tri des points par y
        if(points_tri[0].get_y()> points_tri[1].get_y())std::swap(points_tri[0],points_tri[1]);
        if(points_tri[0].get_y()> points_tri[2].get_y())std::swap(points_tri[0],points_tri[2]);
        if(points_tri[1].get_y()> points_tri[2].get_y())std::swap(points_tri[1],points_tri[2]);
        
        
        float hauteur = points_tri[2].get_y() - points_tri[0].get_y();
         
        
        //Remplissage de la partie haute du triangle (de tri[0].get_y() à tri[1].get_y())
        for(int i = points_tri[0].get_y(); i <= points_tri[1].get_y(); i++){
            float hauteur_sspartie=points_tri[1].get_y() - points_tri[0].get_y()+1;
            
            float alpha = (float)((float)i-points_tri[0].get_y())/hauteur;              
            float beta = (float)((float)i-points_tri[0].get_y())/hauteur_sspartie;
              
            Point p1(points_tri[0].get_x()+(points_tri[2].get_x()-points_tri[0].get_x())*alpha,i);
            Point p2(points_tri[0].get_x()+(points_tri[1].get_x()-points_tri[0].get_x())*beta,i);
             
            
            if(p1.get_x()>p2.get_x())
                std::swap(p1,p2);
              
            line(p1,p2,img,c);
        }
        
        
        //Remplissage de la partie basse du triangle (de tri[2].get_y() à tri[1].get_y())
        for(int i = points_tri[1].get_y(); i <= points_tri[2].get_y(); i++){
            float hauteur_sspartie=points_tri[2].get_y() - points_tri[1].get_y()+1;
            
            float alpha = (float)((float)i-points_tri[0].get_y())/hauteur;              
            float beta = (float)((float)i-points_tri[1].get_y())/hauteur_sspartie;
              
            Point p1(points_tri[0].get_x()+(points_tri[2].get_x()-points_tri[0].get_x())*alpha,i);
            Point p2(points_tri[1].get_x()+(points_tri[2].get_x()-points_tri[1].get_x())*beta,i);
             
            
            if(p1.get_x()>p2.get_x())
                std::swap(p1,p2);
              
            line(p1,p2,img,c);
        }
}

void printTriangle(Object &obj, TGAImage &img){
    
    std::vector<Point> points = obj.get_points();
    std::vector<std::vector<int>> faces = obj.get_faces();
    
 
    
    for(std::vector<int> triangle : faces){
        TGAColor c(std::rand()%255,std::rand()%255,std::rand()%255,255);
        
        std::vector<Point> points_tri;
        
        for(int i : triangle){
            Point p(SIZE/2+SIZE/2*points.at(i).get_x(),SIZE/2+SIZE/2*-points.at(i).get_y());
            points_tri.push_back(p);
        }
        
       traceTriangle(points_tri,img,c);
        
    } 
}

int main(int argc, char** argv) {
	TGAImage image(SIZE, SIZE, TGAImage::RGB);
    
	image.flip_vertically(); // Origin at the left bottom corner
           
    //Object object("./ressources/african_head/african_head.obj");
    Object object("./ressources/diablo3_pose/diablo3_pose.obj");
    //Object object("./ressources/boggie/body.obj");
    
    printPoint(object,image,white);
    
    //Save image
	image.write_tga_file("output_point.tga");
    
    
    image.clear();
     
    printLine(object,image,white);
    
    //Save image
	image.write_tga_file("output_line.tga");
    
    
    image.clear();
    printTriangle(object,image);
     
    //Save image
	image.write_tga_file("output_triangle.tga");
    
    
	return 0;
}


