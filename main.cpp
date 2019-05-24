
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

/*
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
*/

vec3 reflect(const vec3 &v, const vec3 &n) {
    return 2*dot(v,n)*n - v;
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

vec3 spec(light font, hit_record &rec, ray &r, camera cam){
    vec3 direct = cam.origin-rec.p;
    vec3 normal = rec.normal;
    vec3 lig = font.pos-rec.p;
    direct.make_unit_vector();
    lig.make_unit_vector();
    vec3 reflt = 2*(dot(lig, normal))*normal - lig;
    float res = dot(reflt, direct);
    if(res>0){
        return rec.mat.color*font.cor*pow(res, 128);
    }
    return vec3(0.0,0.0,0.0);
}

vec3 phong(hit_record& result, const camera &cam, light font){
    //cout<<"COR: "<<acertos.getColor().x()<<" "<<acertos.getColor().y()<<" "<<acertos.getColor().x()<<endl;
        vec3 lig = font.pos - result.p;
        vec3 direct = cam.origin- result.p;
        vec3 normal = result.normal;

        direct.make_unit_vector();
        normal.make_unit_vector();
        lig.make_unit_vector();
        
        float cos = max(0.0f, dot(normal, lig));

        vec3 base = result.mat.color*result.mat.ke*font.cor;
        vec3 dif = vec3(0,0,0);
        vec3 specular = vec3(0,0,0);
        vec3 reflection = reflect(lig, normal);
        
        if(cos>0.0){
            dif = result.mat.kd * result.mat.color * font.cor * cos;
            specular = result.mat.ks * result.mat.color * font.cor * pow(max(0.0f, dot(direct, reflection)), 128.0);
        }
        vec3 cor = (dif + specular + base);
        return cor;
}

vec3 getColor(ray *r, camera& cam, light& font){
    hit_record result;
    if(world->hit((*r), 0.0, MAXFLOAT, result)){
        hit_record rec;
        if(world->hit(ray(result.p, font.pos-result.p), 0.001,FLT_MAX, rec)) {//Checa se existe um caminho livre até a luz
            return vec3(0.0,0.0,0.0);
        }
        return phong(result, cam, font);
    }
    vec3 unitario = unit_vector(r->direction());
    float t=0.5*(unitario.y()+1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.0, 0.3, 1.0);
}

int main() {
    int x = 1920;
    int y = 1080;
    int ns = 1;
    ofstream out("out.ppm");
    //vec3 lf(-2.0, -1.0, -1.0), origin(0,0,0), horizontal(4.0, 0.0, 0.0), vertical(0,2,0);
    out<<"P3"<<endl<<x<<" "<<y<<endl<<255<<endl;
    sphere *list[2];
    float raio = cos(PI/4.0);
    //float kd, float ks, float ke, float alpha
    list[0] = new sphere(vec3(-raio+2,0.6,-1),raio, new material(vec3(1.0, 0.0, 0.0), 0.2, 0.5, 0.6, 0.4));
    list[1] = new sphere(vec3(0,-100.5,-1),100, new material(vec3(1.0, 1.0, 0.1),  0.2, 0.5, 0.6, 1.0));
    list[2] = new sphere(vec3(raio,1.0, -2.0 ),raio, new material(vec3(0.2, 0.5, 1.0),  0.2, 0.5, 0.6, 1.0));
    world = new hitable_list(list, 3);
    light ligthcenter(vec3(1.0,1.0,1.0), vec3(raio, 6, 4));
    //(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect
    camera cam(vec3(raio+5, 2, 4), vec3(raio,0,-1), vec3(0,1,0), 30, float(x)/float(y), 1.0);//Pos de origem, Ponto de mira, vetor perpendicular, Abertura de lente(FOV) e Aspect
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
