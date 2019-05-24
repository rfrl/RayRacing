
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

//hitable *world;

float max(float a, float b){
    if(a>b) return a;
    return b;
}
vec3 reflect(const vec3 &v, const vec3 &n) {
    return 2*dot(v,n)*n - v;
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

        vec3 base = result.mat->color*result.mat->ke*font.cor;
        vec3 dif = vec3(0.0,0.0,0.0);
        vec3 specular = vec3(0.0,0.0,0.0);
        vec3 reflection = 2*dot(lig, normal)*normal - lig;
        
        if(cos>0.0){
            dif = result.mat->kd * result.mat->color * font.cor * cos;
            specular = result.mat->ks * result.mat->color * font.cor * pow(max(0.0, dot(direct, reflection)), 128.0);
        }
        vec3 cor = (dif + specular + base)*result.mat->alpha;
        return cor;
}

vec3 getColor(ray &r, camera& cam, light& font, const hitable *world){
    hit_record result;
    if(world->hit(r, 0.0000001, numeric_limits<float>::max(), result)){
        hit_record rec;
        if(world->hit(ray(result.p, font.pos-result.p), 0.001,FLT_MAX, rec)) {//Checa se existe um caminho livre atÃ© a luz
            return vec3(0.0,0.0,0.0);
        }
        return phong(result, cam, font);
    }
    vec3 unitario = unit_vector(r.direction());
    float t=0.5*(unitario.y()+1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.0, 0.3, 1.0);
}

int main() {
    int x = 1920;
    int y = 1080;
    int ns = 2;
    ofstream out("out.ppm");
    //vec3 lf(-2.0, -1.0, -1.0), origin(0,0,0), horizontal(4.0, 0.0, 0.0), vertical(0,2,0);
    out<<"P3"<<endl<<x<<" "<<y<<endl<<255<<endl;
    hitable *list[3];
    float raio = cos(PI/4.0);
    //vec3 color, float kd, float ks, float ke, float alpha
    list[0] = new sphere(vec3(-raio,1,-1),raio, new material(vec3(1.0, 0.3, 0.8), 1.0, 1.0, 1.0, 1.0));
    list[1] = new sphere(vec3(raio,-100.5,-1.0),100, new material(vec3(0.4, 1.0, 0.1),  0.7, 0, 1.0, 1.0));
    list[2] = new sphere(vec3(raio,1.0, -1.0 ),raio, new material(vec3(0.2, 0.5, 1.0),  1.0, 1.0, 1.0, 1.0));
    hitable *world = new hitable_list(list, 3);
    light ligthcenter(vec3(1.0, 1.0, 1.0), vec3(-5.0, 1000.0, 701.0));
    //(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect
    vec3 lookfrom  = vec3(raio+5, 3, 4), lookat = vec3(raio,0,-1);
    camera cam(lookfrom, lookat, vec3(0,1,0), 30, float(x)/float(y), 0.2 , (lookfrom-lookat).length());//Pos de origem, Ponto de mira, vetor perpendicular, (FOV) , Aspect, Abertura de lente e Distancia da lente
    for(float i=y-1; i>=0; i--){
        for(float j=0.0; j<x; j++){
            vec3 col(0.0,0.0,0.0);
            for(int s = 0; s<ns; s++){
                float u = float(j+drand48())/(1.0*x);
                float v = float(i+drand48())/(1.0*y);
                ray r = cam.get_ray(u, v);
                col += getColor(r, cam, ligthcenter, world);
            }
            col /= float(ns);
            //col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int rn = 255.99*col[0];
            int gn = 255.99*col[1];
            int bn = 255.99*col[2];
            out<<rn<<" "<<gn<<" "<<bn<<endl;
        }
    }  
}