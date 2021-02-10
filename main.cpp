#include "tgaimage.h"
#include "object.h"
#include "matrix.h"
#include <limits>
#include <iostream>


#define SIZE 1000
#define DEPTH 255

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue   = TGAColor(0, 0,   255,   255);

Point3d camera(0,0,3);
Point3d light(0,0,-1);



float max(float x, float y){
    return x>y?x:y;
}

float min(float x, float y){
    return x<y?x:y;
}

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

void line(Point2d p0, Point2d p1, TGAImage &image, TGAColor color){
line(p0.get_x(), p0.get_y(), p1.get_x(), p1.get_y(), image, color);
}


void printPoint2d(Object &obj, TGAImage &img, const TGAColor &c){
    for(Point3d p : obj.get_points()){
        img.set(SIZE/2+SIZE/2*p.get_x(),SIZE/2+p.get_y()*SIZE/2,c); 
    }
}


Point3d barycentric(std::vector<Point3d> &pts, Point2d P){
    Point3d p1(pts[2].get_x()-pts[0].get_x(), pts[1].get_x()-pts[0].get_x(),pts[0].get_x()-P.get_x());
    Point3d p2(pts[2].get_y()-pts[0].get_y(), pts[1].get_y()-pts[0].get_y(),pts[0].get_y()-P.get_y());
    
    Point3d u = p1.cross(p2);
    
    //if(std::abs(u.get_z()<1)) return Point3d(1,1,1);
    
    return Point3d(1.f-(u.get_x()+u.get_y())/u.get_z(), u.get_y()/u.get_z(), u.get_x()/u.get_z());
} 

void printLine(Object &obj, TGAImage &img, const TGAColor &c){
    
    std::vector<Point3d> points = obj.get_points();
    std::vector<std::vector<int>> faces = obj.get_faces();
    
    for(std::vector<int> triangle : faces){
        for(int cpt = 0; cpt<3; cpt++){
            line((int)(SIZE/2+SIZE/2*points.at(triangle[cpt]).get_x()),(int)(SIZE/2+SIZE/2*points.at(triangle[cpt]).get_y()),(int)(SIZE/2+SIZE/2*points.at(triangle[(cpt+1)%3]).get_x()),(int)(SIZE/2+SIZE/2*points.at(triangle[(cpt+1)%3]).get_y()), img, c);
        }
    } 
}


void traceTriangle(std::vector<Point3d> points_tri, std::vector<Point3d> points_text,TGAImage &img, float light_intensity,  double * zbuffer[], Object &obj){
        //Tri des points par y
        if(points_tri[0].get_y()> points_tri[1].get_y()){std::swap(points_tri[0],points_tri[1]);std::swap(points_text[0],points_text[1]);}
        if(points_tri[0].get_y()> points_tri[2].get_y()){std::swap(points_tri[0],points_tri[2]);std::swap(points_text[0],points_text[2]);}
        if(points_tri[1].get_y()> points_tri[2].get_y()){std::swap(points_tri[1],points_tri[2]);std::swap(points_text[1],points_text[2]);}
        
        //Barycentic Method
        Point2d boxmin(img.get_width()-1, img.get_height()-1);
        Point2d boxmax(0,0);
        Point2d clamp(img.get_width()-1,img.get_height()-1);
        
        for(int i =0; i<3; i++){ 
            boxmin.set_x(max(0,min(boxmin.get_x(),points_tri[i].get_x())));
            boxmin.set_y(max(0,min(boxmin.get_y(),points_tri[i].get_y())));
            boxmax.set_x(min(clamp.get_x(),max(boxmax.get_x(),points_tri[i].get_x())));
            boxmax.set_y(min(clamp.get_y(),max(boxmax.get_y(),points_tri[i].get_y())));
        }
            
        double pz;
        
        for(int px = boxmin.get_x(); px<=boxmax.get_x();px++){  
            for(int py = boxmin.get_y(); py<=boxmax.get_y();py++){
                Point3d bc_screen = barycentric(points_tri,Point2d(px,py));
                if(bc_screen.get_x()<0 || bc_screen.get_y()<0 || bc_screen.get_z() < 0) continue;
                
                pz=points_tri[0].get_z()*bc_screen.get_x();
                pz+=points_tri[1].get_z()*bc_screen.get_y();
                pz+=points_tri[2].get_z()*bc_screen.get_z();
                 
                if(zbuffer[px][py]<pz){
                    TGAColor color; 
                    
                    float pu = 0, pv =0;
                
                        
                    pu=points_text[0].get_x()*bc_screen.get_x()+points_text[1].get_x()*bc_screen.get_y()+points_text[2].get_x()*bc_screen.get_z();
                    pv=points_text[0].get_y()*bc_screen.get_x()+points_text[1].get_y()*bc_screen.get_y()+points_text[2].get_y()*bc_screen.get_z();
                    //pw=points_text[0].get_z()*bc_screen.get_x()+points_text[1].get_z()*bc_screen.get_y()+points_text[2].get_z()*bc_screen.get_z();
                    
                    color= obj.get_color(Point2d(pu,pv),light_intensity);
                    
                    zbuffer[px][py] = pz;
                    img.set(px,py,color);
                }
            }
        }
}


void printTriangle(Object &obj, TGAImage &img, bool shading){
    
    std::vector<Point3d> points = obj.get_points();
    std::vector<Point3d> all_points_text = obj.get_textures();
    std::vector<std::vector<int>> faces = obj.get_faces();
    std::vector<std::vector<int>> faces_text = obj.get_texture_faces();
     
    double ** zbuffer = new double*[SIZE];
 
    for(int i = 0; i<SIZE; i++){
        zbuffer[i]=new double[SIZE];
        for(int j = 0; j<SIZE; j++){
            zbuffer[i][j]=std::numeric_limits<int>::min();
        }
    }
    
    int fcpt = 0;
    
    for(std::vector<int> triangle : faces){ 
        std::vector<Point3d> points_tri;
        std::vector<Point3d> points_text;
        std::vector<Point3d> points_screen;
        
        for(int i : triangle){
            Point3d p(points.at(i).get_x(),points.at(i).get_y(),points.at(i).get_z());
            float c= camera.get_z();
                         
            p.set_x(SIZE/2+SIZE/3*(p.get_x()/(float)(1-(p.get_z()/c))));
            p.set_y(SIZE/2+SIZE/3*(p.get_y()/(float)(1-(p.get_z()/c))));
            p.set_z(SIZE/2*(p.get_z()/(float)(1-(p.get_z()/c))));
             
            points_screen.push_back(p);
            points_tri.push_back(p);
        }
        
        
        for(int i : faces_text.at(fcpt)){
            points_text.push_back(all_points_text.at(i));
        }
        
        fcpt++;
        
        
          
         
         Point3d normal = (points_screen[2].minus(points_screen[0])).cross(points_screen[1].minus(points_screen[0])); 
         
         normal.normalize();
         
         float light_intensity = -light.dotproduct(normal);
          
         if(light_intensity>0 && shading){
         
            traceTriangle(points_tri,points_text,img,light_intensity,zbuffer,obj);
        }
         
         if(!shading){
             
            traceTriangle(points_tri,points_text,img,light_intensity,zbuffer,obj);
         }
         
         
         
        
        
    } 
}

void printTestYBuffer(TGAImage &img){
     
    line(20,34,744,400, img, red);
    line(120,434,444,400, img, green);
    line(330,463,594,200, img, blue);
    line(10,10,790,10, img, white);
}

void rasterize(Point2d p0, Point2d p1, TGAImage &img,const TGAColor &c, int ybuffer[]){
    if(p0.get_x()>p1.get_x()){
        std::swap(p0,p1);
    }
    
    for(int x = p0.get_x(); x <= p1.get_x(); x++){
        float t = (x-p0.get_x())/(float)(p1.get_x()-p0.get_x());
        
        int y = p0.get_y()*(1.-t)+p1.get_y()*t+0.5;
         
        if(x < SIZE && x >0 && ybuffer[x]<y){ 
            ybuffer[x]=y; 
            img.set(x,0,c);
        }
    }
}
     
void rasterizeTest(TGAImage &image2){
    
    int ybuffer[SIZE];
    
    for(int i = 0; i<SIZE; i++){
        ybuffer[i]=std::numeric_limits<int>::min();
    }
    
    rasterize(Point2d(20,34),Point2d(744,400),image2,red,ybuffer);
    rasterize(Point2d(120,434),Point2d(444,400),image2,green,ybuffer);
    rasterize(Point2d(330,464),Point2d(594,200),image2,blue,ybuffer);
    
}
  
  

int main(int argc, char** argv) {
	TGAImage image(SIZE, SIZE, TGAImage::RGB);
    
	image.flip_vertically(); // Origin at the left bottom corner
           
    Object object("./ressources/african_head/african_head.obj","./ressources/african_head/african_head_diffuse.tga");
    //Object object("./ressources/diablo3_pose/diablo3_pose.obj","./ressources/diablo3_pose/diablo3_diffuse.tga");
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
    printTriangle(object,image,true);
    image.flip_vertically();
     
    //Save image
	image.write_tga_file("output_triangle.tga");
    
    image.clear(); 
    printTestYBuffer(image);  
    image.flip_vertically();
     
    //Save image
	image.write_tga_file("output_ybuffer.tga");
    
     
	TGAImage image2(SIZE, 16, TGAImage::RGB);
    rasterizeTest(image2); 
    
	return 0;
}
