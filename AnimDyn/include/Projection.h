#ifndef H_PR0J3CT10N_H
#define H_PR0J3CT10N_H

#include "Mathril.h"

namespace proj
{
    extern Mat4 perspective;
    extern float fov;
    extern float near;
    extern float far;
    extern float aspectRatio;
    void setPerspective(float fov, float near, float far, float aspectRatio);

    extern Mat4 orthogonal;
    void setOrthogonal(float near, float far, float right, float left, float top, float bottom);
}


#endif
