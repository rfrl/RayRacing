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

#ifndef SPHEREH
#define SPHEREH
#include <stdlib.h>
#include "hitable.h"

class sphere: public hitable  {
    public:
        sphere() {}
        sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(*m)  {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        vec3 center;
        float radius;
        material mat_ptr;


    bool intersect(float max, hit_record &rec, ray *r) {
        vec3 distance = r->origin()- this->center;
        float a = dot(r->direction(), r->direction());
        float b = 2 * dot(distance, r->direction());
        float c = dot(distance, distance) - (this->radius * this->radius);
        float delta = b * b - 4 * a * c;
        if (delta > 0) {
            float temp = (-1 * b - sqrt(delta) ) / (2.0 * a);
            if(temp > 0.1 && temp < max){
                temp += 0.000000001;
                rec.t = temp;
                rec.p = r->point_at_parameter(temp);
                vec3  op = rec.p-(this->center);
                op.make_unit_vector();
                rec.normal = op;
                return true;
            }
            temp = (-1 * b + sqrt(delta) ) / (2.0 * a);
            if(temp > 0.1 && temp < max){
                temp += 0.000000001;
                rec.t = temp;
                rec.p = r->point_at_parameter(temp);
                vec3  op = rec.p-(this->center);
                op.make_unit_vector();
                rec.normal = op;
                return true;
            }
        } else if ( delta == 0) {
            float temp = -1 * b / 2 * a;
            if( temp > 0 && temp < max){
            temp = temp + 0.00000000001;
            rec.t = temp;
            rec.p = r->point_at_parameter(temp);
            vec3  op = rec.p-(this->center);
            op.make_unit_vector();
            rec.normal = op;
            return true;
            }
        }
        return false;
}
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat = mat_ptr;
            return true;
        }
    }
    return false;
}


#endif



