#ifndef H_3N61N3_H
#define H_3N61N3_H

#include <map>
#include <iostream>
#include <time.h>
#include <vector>
#include "Win.h"
#include "Projection.h"
#include "ObjectRendererAnimated.h"


class Engine
{
private:
    float fps = 120.0f;
    float ms_per_update=1000.0f/fps;

    int framesCounter=0;
    double currentFPS = 0; 

    Win window;


    float translateForward, translateSide;
  	float transVal = 0.5f; // 0.05
  	float rotx, roty;

    void initialize();
    void update();
    void input();
    void render(double);

    std::map<std::string, Mesh> meshMap; 
    std::map<std::string, Texture> texMap;
    Camera cam;
    DirectionalLight sun;

    bool hold=true;
    bool wireframe=false;
    bool updateTerrain=true;

    MeshAnimated meshAnimated;
    ObjectRendererAnimated objectRendererAnimated;
    std::vector<ObjectAnimated> objectsAnimated;

    std::string dynPath, texPath;

public:
    Engine();
    Engine(std::string dynPath, std::string texPath);
    ~Engine();
    void loop();
};

#endif
