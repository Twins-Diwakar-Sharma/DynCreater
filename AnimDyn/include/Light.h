#ifndef H_PR4K4SH_H
#define H_PR4K4SH_H

#include "Camera.h"
#include "Mathril.h"
#include "Projection.h"
class DirectionalLight
{
private:
    Vec3 direction;
    Vec3 color;
    Quat spin;
public:
    DirectionalLight();
    DirectionalLight(DirectionalLight&);
    DirectionalLight(DirectionalLight&&);
    DirectionalLight(Vec3&& color);
    DirectionalLight(Vec3& color);
    ~DirectionalLight();

    Vec3& getColor();
    Vec3& getDirection();
    void setColor(float r, float g, float b);
    DirectionalLight& operator=(DirectionalLight&);
    DirectionalLight& operator=(DirectionalLight&&);
    void rotate(float x, float y, float z);
    Quat& getSpin();
    Vec3& getPosition();
};

#endif
