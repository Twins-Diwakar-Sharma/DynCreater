#include "Light.h"
#include "Projection.h"
#include <cwchar>

DirectionalLight::DirectionalLight()
{
    direction[2] = -1;
    spin[0] = 1;
}

DirectionalLight::DirectionalLight(DirectionalLight& light)
{
    direction = light.direction;
    color = light.color;
    spin = light.spin;
}

DirectionalLight::DirectionalLight(DirectionalLight&& light)
{
    direction = std::move(light.direction);
    color = std::move(light.color);
    spin = std::move(light.spin);
}

DirectionalLight::DirectionalLight(Vec3& color)
{
    this->direction = Vec3(0,0,-1);
    this->color = color; 
    this->spin[0] = 1;
}

DirectionalLight::DirectionalLight(Vec3&& color)
{
    this->color = std::move(color);
    this->direction = Vec3(0,0,-1);
    this->spin[0] = 1;
}

DirectionalLight::~DirectionalLight()
{
}

Vec3& DirectionalLight::getColor()
{
    return color;
}

Vec3& DirectionalLight::getDirection()
{
    return direction;
}

void DirectionalLight::setColor(float r, float g, float b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
}


DirectionalLight& DirectionalLight::operator=(DirectionalLight& dl)
{
    if( this == &dl ) [[unlikely]] 
    	return *this;

    color = dl.color;
    direction = dl.direction;
    return *this;
}

DirectionalLight& DirectionalLight::operator=(DirectionalLight&& dl)
{
    color = std::move(dl.color);
    direction = std::move(dl.direction);
    return *this;
}


void DirectionalLight::rotate(float x, float y, float z)
{
	float rad = M_PI / 180.0f;
	float radx = x / 2 * rad, rady = y / 2 * rad, radz = z / 2 * rad;
	float cx = std::cos(radx), sx = std::sin(radx);
	float cy = std::cos(rady), sy = std::sin(rady);
	float cz = std::cos(radz), sz = std::sin(radz);

  spin = Quat(cy, 0, sy, 0) * spin * Quat(cx, sx, 0, 0) * Quat(cz, 0, 0, sz);

  Quat forward(0,0,0,-1);
  forward = spin * forward * (~spin);
  for(int i=0; i<3; i++)
    direction[i] = forward[i+1];

}

Quat& DirectionalLight::getSpin(){
  return spin;
} 
