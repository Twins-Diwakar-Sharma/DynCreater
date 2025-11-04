#include "Engine.h"

Engine::Engine()
{}

Engine::Engine(std::string dynPath, std::string texPath) : dynPath(dynPath), texPath(texPath), meshAnimated(dynPath)
{}

Engine::~Engine()
{}

void Engine::loop()
{
    clock_t prev = clock();
    double elapsed = 0, lag = 0;
    clock_t curr = clock();
    initialize();
    while(!window.close()) // ! window close
    {
        elapsed = ((double)curr - (double)prev) * 1000 / CLOCKS_PER_SEC;
        lag += elapsed;
        prev = clock();

        input();
/*      while(lag >= ms_per_update)
        {
            update();
            lag -= ms_per_update;
        }
*/
        update();
        render(lag/ms_per_update);
        curr = clock();

        framesCounter++;
        if(lag >= 1000)
        {
          lag = 0;
          framesCounter = 0;
        }
  
    }
    
}    

void Engine::initialize()
{
    proj::setPerspective(60.0f,0.1f,1000.0f,window.getAspect());
    glClearColor(0.9f, 0.9f, 0.95f, 1.0f);
    //glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    sun = DirectionalLight((1.0f/255.0f)*Vec3(255,255,255));
    sun.rotate(-30, 0, 0);

    
    texMap.try_emplace("texture", texPath, TEX_DEF);
    objectsAnimated.emplace_back(meshAnimated, texMap["texture"]);
    objectsAnimated[0].setPosition(0,0,-10);
    objectsAnimated[0].setAnimation(0);
  
    objectsAnimated[0].animate();
}

void Engine::input()
{
    window.handleKey(translateForward, translateSide, transVal);
    window.handleMouse(rotx, roty); 
    window.handleHold(hold);
    window.handleWireframe(wireframe);
    
    if(Win::charInputEvent)
    {
      Win::charInputEvent = false;
      if(Win::currentChar == 'n')
        objectsAnimated[0].animateNext();
    }

    window.pollEvents(); 
}

void Engine::update()
{
  if(hold)
  {
    cam.setPosition(0,0,0);
    rotx = 0; roty = 0;
    cam.spin[0] = 1; cam.spin[1] = 0; cam.spin[2] = 0; cam.spin[3] = 0;
  }
  else
  {
    cam.rotate(rotx, roty, 0);
    rotx = 0;	roty = 0;
    cam.translate(translateForward, translateSide);
    objectsAnimated[0].animate();
  }


  translateForward = 0; translateSide = 0;
  
}

void Engine::render(double dt)
{
  auto [winW,winH] = window.getDimensions();
  glViewport(0,0,winW,winH);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  objectRendererAnimated.render(objectsAnimated, cam, sun);
  if(wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  window.swap();

}
