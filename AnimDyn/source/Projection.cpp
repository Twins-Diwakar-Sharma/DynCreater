#include "Projection.h"

Mat4 proj::perspective;
float proj::near;
float proj::far;
float proj::fov;
float proj::aspectRatio;

void proj::setPerspective(float fov, float near, float far, float aspectRatio)
{
    proj::near = near;
    proj::far = far;
    proj::fov = fov;
    proj::aspectRatio = aspectRatio;

    float radFov = fov *  M_PI / (180.0f);

    float d = 1.0f / (tan(radFov / 2.0f));
    perspective[0][0] = d / aspectRatio;
    perspective[1][1] = d;
    perspective[2][2] = (near + far) / (near - far);
    perspective[2][3] = (2 * near * far) / (near - far);
    perspective[3][2] = -1.0f;
}


Mat4 proj::orthogonal;
void proj::setOrthogonal(float near, float far, float right, float left, float top, float bottom)
{
    orthogonal[0][0] = 2/(right-left);
    orthogonal[0][3] = -(right+left)/(right-left);
    orthogonal[1][1] = 2/(top-bottom);
    orthogonal[1][3] = -(top+bottom)/(top-bottom);
    orthogonal[2][2] = -2/(far-near);
    orthogonal[2][3] = -(far+near)/(far-near);
    orthogonal[3][3] = 1;
}
