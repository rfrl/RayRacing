
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include <iostream>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <limits>
#include <fstream>

using namespace std;
const float MAXFLOAT = FLT_MAX;
hitable *world;
vec3 SphereCenter(0,0,-1);
float SphereRadius = 0.5;

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

vec3 getColor(ray& r){
    float t = isSphere(SphereCenter, SphereRadius, r);
    hit_record result;
    if(world->hit(r, 0.0, MAXFLOAT, result)){
        return 0.5*vec3(result.normal.x()+1, result.normal.y()+1, result.normal.z()+1);
    }
    vec3 unitario = unit_vector(r.direction());
    t=0.5*(unitario.y()+1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.2, 0.3, 0.2);
}

int main() {
    int x = 400;
    int y = 400;
    int ns = 10;
    ofstream out("out.ppm");
    //vec3 lf(-2.0, -1.0, -1.0), origin(0,0,0), horizontal(4.0, 0.0, 0.0), vertical(0,2,0);
    out<<"P3"<<endl<<x<<" "<<y<<endl<<255<<endl;
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1),0.5, new dielectric(1.5));
    list[1] = new sphere(vec3(0,-100.5,-1),100, new dielectric(1.2));
    list[2] = new sphere(vec3(1,0,-1),0.5, new lambertian(vec3(1,1,1)));
    world = new hitable_list(list, 3);
    //(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect
    camera cam(vec3(-2,2,1), vec3(0,0,-1), vec3(0,1,0), 90, float(x)/float(y));//Origem, Centro da figura, vetor perpendicular, Abertura de lente(FOV) e Aspect
    for(float i=y-1; i>=0; i--){
        for(float j=0.0; j<x; j++){
            vec3 col(0.0,0.0,0.0);
            for(int s = 0; s<ns; s++){
                float u = float(j+drand48())/(1.0*x);
                float v = float(i+drand48())/(1.0*y);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += getColor(r);
            }
            col /= float(ns);
            int rn = 255.99*col[0];
            int gn = 255.99*col[1];
            int bn = 255.99*col[2];
            out<<rn<<" "<<gn<<" "<<bn<<endl;
        }
    }
    
    
}
