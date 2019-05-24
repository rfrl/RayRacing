//==================================================================================================
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================

#ifndef MATERIALH
#define MATERIALH 

struct hit_record;
#include <stdlib.h>
#include <cstdlib>
#include "ray.h"
#include "hitable.h"
#include "camera.h"


float schlick(float cosine, float ref_idx) {
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else 
        return false;
}


vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}


class material{
    public:
        material(vec3 color, float kd, float ks, float ke, float alpha){
            this->ke = ke;
            this->kd = kd;
            this->ks = ks;
            this->alpha = alpha;
            this->color = color;
        }
        material(){
            this->ke = 0;
            this->kd = 0;
            this->ks = 0;
            this->alpha = 0;
            this->color = vec3(0,0,0);
        }
        float ke;
        float kd;
        float ks;
        float alpha;
        vec3 color;
};

#endif




