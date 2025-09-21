#include "Convertor.h"


int main(int argc, char** argv)
{
  if(argc != 2)
  {
    std::cerr << "Incorrect arguements, correct usage: " << std::endl;
    std::cerr << "DynMaker /path/to/gltf/file " <<std::endl;
    return -1;
  }

  std::string fullPath = std::string(argv[1]);
  std::string outputPath = dyn::convert(fullPath);  

}
