#ifndef H_R3nD3r3R_AN1M473D_H
#define H_R3nD3r3R_AN1M473D_H


#include "Mathril.h"
#include "ShaderProgram.h"
#include "Projection.h"
#include "ObjectAnimated.h"
#include <vector>

class ObjectRendererAnimated
{
private:
    ShaderProgram shaderProgram;
public:
    ObjectRendererAnimated();
    ~ObjectRendererAnimated();  
    void render(std::vector<ObjectAnimated>& objects, Camera& cam, DirectionalLight& sun);
};

#endif
