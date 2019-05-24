
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include <iostream>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <limits>
#include <fstream>

using namespace std;
const float MAXFLOAT = FLT_MAX;
hitable_list *world;


bool isShadow(ray *r,  light font){
    hit_record rec;
    double distancelight = getDistance(font.pos, r->origin());
    double distanceobj;
    int ligthc = 1e7;
    for(int i =0; i< world->list_size; i++){
        sphere obj = (*world->list[i]);
        distanceobj = getDistance(obj.center, r->origin());
        if(distancelight>distanceobj){
            if(obj.intersect(ligthc, rec, r)){
                return 1;
            }
        }
    }
    return 0;
}

vec3 reflection(vec3 a, vec3 b){
    float d = dot(a, b);
    d = d*2;
    b = b*d;
    a = a-b;
    return a;
}

vec3 difuse(light font, hit_record& gets){
    vec3 inter = font.pos-gets.p;
    inter.make_unit_vector();
    float d = dot(gets.normal, inter);
    if(d<=0){
        return vec3(0,0,0);
    }else{
        return font.cor*d;
    }
}


float isSphere(vec3& center, float raio, ray& r){
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0* dot(oc, r.direction());
    float c = dot(oc, oc) - raio*raio;
    float delta = b*b-4*a*c;
    if(delta<0){
        return -1.0;
    }
    return (-b - sqrt(delta))/2.0*a; 
}

vec3 spec(light font, hit_record &rec, ray &r){
    vec3 inter  = font.pos-rec.p;
    inter.make_unit_vector();
    vec3 refl = reflection(inter, rec.normal);
    refl.make_unit_vector();
    float d = dot(refl, r.direction());
    if(d<0) d = 0;
    d = pow(d, rec.mat.alpha);
    return font.cor*d;
}

vec3 phong(hit_record& result, const camera &cam, light font, ray *r){
    //cout<<"COR: "<<acertos.getColor().x()<<" "<<acertos.getColor().y()<<" "<<acertos.getColor().x()<<endl;
        vec3 perc = difuse(font, result);
        vec3 dif = perc*result.mat.kd;

        vec3 base = result.mat.color*result.mat.ke;
        
        vec3 specular = spec(font, result, (*r));
        specular = specular*result.mat.ks;
        
        
        vec3 cor = (dif + base + specular);
        return cor;
}

vec3 getColor(ray *r, camera& cam, light& font){
    hit_record result;
    if(world->hit((*r), 0.0, MAXFLOAT, result)){
        hit_record rec;
        if(world->hit(ray(result.p, font.pos-result.p), 0.001,FLT_MAX, rec)) {
            return vec3(0.0,0.0,0.0);
        }
        return phong(result, cam, font, r);
    }
    vec3 unitario = unit_vector(r->direction());
    float t=0.5*(unitario.y()+1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.2, 0.3, 0.2);
}

int main() {
    int x = 1920;
    int y = 1080;
    int ns = 10;
    ofstream out("out.ppm");
    //vec3 lf(-2.0, -1.0, -1.0), origin(0,0,0), horizontal(4.0, 0.0, 0.0), vertical(0,2,0);
    out<<"P3"<<endl<<x<<" "<<y<<endl<<255<<endl;
    sphere *list[2];
    //float kd, float ks, float ke, float alpha
    list[0] = new sphere(vec3(4.0,1.0,-1),0.5, new material(vec3(1.0, 0.0, 0.0),  0.2, 0.3, 0.6, 1.0));
    list[1] = new sphere(vec3(0,-1000,-1),1000, new material(vec3(1.0, 1.0, 0.1),  0, 0, 1, 0));
    list[2] = new sphere(vec3(1.0,1.0,-1),0.7, new material(vec3(0.2, 0.5, 1.0),  0.2, 0.3, 0.6, 1.0));
    world = new hitable_list(list, 3);
    light ligthcenter(vec3(1.0,1.0,1.0), vec3(2.0,1000.0,-1.0));
    //(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect
    camera cam(vec3(-1,2,1), vec3(0,1,-1), vec3(0,1,0), 90, float(x)/float(y), 1.2);//Pos de origem, Ponto de mira, vetor perpendicular, Abertura de lente(FOV) e Aspect
    for(float i=y-1; i>=0; i--){
        for(float j=0.0; j<x; j++){
            vec3 col(0.0,0.0,0.0);
            for(int s = 0; s<ns; s++){
                float u = float(j+drand48())/(1.0*x);
                float v = float(i+drand48())/(1.0*y);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += getColor(&r, cam, ligthcenter);
            }
            col /= float(ns);
            int rn = 255.99*col[0];
            int gn = 255.99*col[1];
            int bn = 255.99*col[2];
            out<<rn<<" "<<gn<<" "<<bn<<endl;
        }
    }  
}
