#include <iostream>
#include "Engine.h"

int main(int argc, char** argv)
{

  if(argc < 2 || argc > 3)
  {
    std::cerr << "Incorrect arguements, correct usage: " << std::endl;
    std::cerr << "viewerDyn /path/to/dyn/file (optional)/path/to/texture/file" <<std::endl;
    return -1;
  }


  std::cout << "hello from viewer" << std::endl;
  std::string dynPath = std::string(argv[1]);
  std::string texPath="";
  if(argc == 3)
  {
    texPath = std::string(argv[2]);
  }

  Engine engine(dynPath, texPath);
  engine.loop();

}
