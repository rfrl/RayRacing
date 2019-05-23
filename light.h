#include <stdlib.h>
#include <cstdlib>
#include "ray.h"
#include "hitable.h"
class light{
    public:
        light(vec3 color, vec3 position){
            this->cor = color;
            this->pos = position;
        }
        vec3 cor;
        vec3 pos;
};