#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include <vector>
#include "tgaimage.h"
#include "renderer.h"

 double ** zbuffer = new double*[SIZE];
 
 void init(){
     
 
    for(int i = 0; i<SIZE; i++){
        zbuffer[i]=new double[SIZE];
        for(int j = 0; j<SIZE; j++){
            zbuffer[i][j]=std::numeric_limits<int>::min();
        }
    }
 }


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


void traceTriangle(std::vector<Point3d> points_tri, std::vector<Point3d> points_text,std::vector<Point3d>  points_vn,TGAImage &img, float light_intensity,  double * zbuffer[], Object &obj){
        //Tri des points par y
        if(points_tri[0].get_y()> points_tri[1].get_y()){std::swap(points_tri[0],points_tri[1]);std::swap(points_text[0],points_text[1]);std::swap(points_vn[0],points_vn[1]);}
        if(points_tri[0].get_y()> points_tri[2].get_y()){std::swap(points_tri[0],points_tri[2]);std::swap(points_text[0],points_text[2]);std::swap(points_vn[0],points_vn[2]);}
        if(points_tri[1].get_y()> points_tri[2].get_y()){std::swap(points_tri[1],points_tri[2]);std::swap(points_text[1],points_text[2]);std::swap(points_vn[1],points_vn[2]);}
        
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
                 
                if(zbuffer[px][py]<=pz){
                    TGAColor color; 
                    
                    float pu = 0, pv =0, pw=0,pul = 0, pvl =0, pwl=0;
                 
                        
                    
                    pu=points_text[0].get_x()*bc_screen.get_x()+points_text[1].get_x()*bc_screen.get_y()+points_text[2].get_x()*bc_screen.get_z();
                    pv=points_text[0].get_y()*bc_screen.get_x()+points_text[1].get_y()*bc_screen.get_y()+points_text[2].get_y()*bc_screen.get_z();
                    pw=points_text[0].get_z()*bc_screen.get_x()+points_text[1].get_z()*bc_screen.get_y()+points_text[2].get_z()*bc_screen.get_z();
                    
                    
                    pul=points_vn[0].get_x()*bc_screen.get_x()+points_vn[1].get_x()*bc_screen.get_y()+points_vn[2].get_x()*bc_screen.get_z();
                    pvl=points_vn[0].get_y()*bc_screen.get_x()+points_vn[1].get_y()*bc_screen.get_y()+points_vn[2].get_y()*bc_screen.get_z();
                    pwl=points_vn[0].get_z()*bc_screen.get_x()+points_vn[1].get_z()*bc_screen.get_y()+points_vn[2].get_z()*bc_screen.get_z();
                    
                    
                    Point3d poin(pul,pvl,pwl); 
                    
                    Point3d light(0,0,-1);
                    
                    float intensity = -poin.dotproduct(light);
                      
                    color= obj.get_color(Point2d(pu,pv),intensity);
                    
                    
                    if(intensity>0){
                    
                    zbuffer[px][py] = pz;
                    img.set(px,py,color);
                    }
                }
            }
        }
}


Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = DEPTH/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = DEPTH/2.f;
    return m;
}

Matrix lookat(Point3d eye, Point3d up) {
    Point3d c(0,0,0);
    
    Point3d z = (eye.minus(Point3d(0,0,0)));
    z.normalize();
    Point3d x = (up.cross(z));
    x.normalize();
    Point3d y = (z.cross(x));
    y.normalize();
    
    Matrix res = Matrix::identity(4);
    for (int i=0; i<3; i++) {
        res[0][i] = x.get(i);
        res[1][i] = y.get(i);
        res[2][i] = z.get(i);
        res[i][3] = -c.get(i);
    }
    return res;
}

void printTriangle(Object &obj, TGAImage &img, bool shading, Point3d eye, Point3d light){
    
    std::vector<Point3d> points = obj.get_points();
    std::vector<Point3d> all_points_text = obj.get_textures();
    std::vector<Point3d> all_vn_text = obj.get_vn();
    std::vector<std::vector<int>> faces = obj.get_faces();
    std::vector<std::vector<int>> faces_text = obj.get_texture_faces();
    std::vector<std::vector<int>> faces_vn = obj.get_vn_faces();
     
   
    
    int fcpt = 0;
    
    Matrix ViewPort   = viewport(SIZE/8, SIZE/8, SIZE*3/4, SIZE*3/4);
    Matrix ModelView  = lookat(eye, Point3d(0,1,0));
    
    Matrix id = Matrix::identity(4);
            
    id[3][2] = -1.f/(eye.minus(Point3d(0,0,0))).norm();
            
    for(std::vector<int> triangle : faces){   
        std::vector<Point3d> points_vn; 
        std::vector<Point3d> points_text;
        std::vector<Point3d> points_screen;
        std::vector<Point3d> points_world;
        
        for(int i : triangle){
            Point3d p(points.at(i).get_x(),points.at(i).get_y(),points.at(i).get_z());
            
            points_world.push_back(p); 
             
            Matrix mat = Matrix::fromP3D(p);
            
            p=(ViewPort*id*ModelView*mat).toP3D();  
            
            
            points_screen.push_back(p);  
        }
        
        
        for(int i : faces_text.at(fcpt)){
            points_text.push_back(all_points_text.at(i));
        }
        
        
        for(int i : faces_vn.at(fcpt)){
            points_vn.push_back(all_vn_text.at(i));
        }
        
        fcpt++;
        
        
          
         
         Point3d normal = (points_world[2].minus(points_world[0])).cross(points_world[1].minus(points_world[0])); 
         
         normal.normalize();
         
         float light_intensity = -light.dotproduct(normal);
          
         if(light_intensity>0 && shading){
         
            traceTriangle(points_screen,points_text,points_vn,img,light_intensity,zbuffer,obj);
        }
         
         if(!shading){
             
            traceTriangle(points_screen,points_text,points_vn,img,light_intensity,zbuffer,obj);
         } 
    } 
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
