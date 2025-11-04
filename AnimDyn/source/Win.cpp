#include "Win.h"

int Win::width = 1366;
int Win::height = 768;
char Win::currentChar = ' ';
bool Win::charInputEvent = false;

Win::Win()
{
    if(!glfwInit())
    {
        std::cerr << "Failed to init glfw" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    win = glfwCreateWindow(width, height, title, NULL, NULL);
    if(win == NULL)
    {
        std::cerr<< " Broken Window :("<<std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(win);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cerr << "Failed to initialize glad" <<std::endl;
    glViewport(0,0,width,height);

    glfwSetFramebufferSizeCallback(win,resize);

    glfwSetWindowPos(win,0,0);
    std::string displayServer = std::string(DISPLAY_SERVER);
    if(displayServer.compare("wayland") == 0)
    {
      glfwSetInputMode(win,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    }
    else
    {
      glfwSetInputMode(win,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
    }

    glfwSetCharCallback(win, char_callback);
}

Win::~Win()
{}

void Win::resize(GLFWwindow* w, int width, int height)
{
    Win::width = width;
    Win::height = height;
    glViewport(0,0,width,height);
    proj::setPerspective(proj::fov,proj::near,proj::far,(float)width/height);
}

bool Win::close()
{
    return glfwWindowShouldClose(win);
}

void Win::terminate()
{
    glfwTerminate();
}

void Win::swap()
{
    glfwSwapBuffers(win);
}

float Win::getAspect()
{
    return (float)Win::width/Win::height;
}

void Win::handleKey(float& t, float& s, float val)
{
    if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		t += val;
	else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		t -= val;

	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		s -= val;
	else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		s += val;

	glfwPollEvents();
}


void Win::handleMouse(float& rx, float& ry)
{
	double x, y;
	glfwGetCursorPos(win, &x, &y);
	ry += -(x - width / 2) * senstivity; 
	rx += -(y - height / 2) * senstivity;
	
	glfwSetCursorPos(win, (double)width / 2, (double)height / 2);
}

void Win::pollEvents()
{
    glfwPollEvents();
}

void Win::handleHold(bool& hold)
{
    if(glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        hold = true;
    if(glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        hold = false;
    
}

void Win::handleTerrain(bool& updateTerrain)
{
    if(glfwGetKey(win, GLFW_KEY_T) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        updateTerrain = true;
    if(glfwGetKey(win, GLFW_KEY_T) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        updateTerrain = false;
}

void Win::handleWireframe(bool& wireframe)
{
    if(glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        wireframe = true;
    if(glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        wireframe = false;
}

std::tuple<int,int> Win::getDimensions()
{
  return {width, height};
}

void Win::char_callback(GLFWwindow* window, unsigned int codepoint)
{
    currentChar = (char)codepoint;
    charInputEvent = true;
}
