#include "Convertor.h"

// Remember:
// During animation:
//  0> select all bones, add k for adding keyframe
//  1> set interpolation to linear (setting cubic will add unecessary data)
// 
// During export:
//  0> select gtlf + bin
//  1> unselect sampling rate (very important, otherwise multiple keyframes)

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
