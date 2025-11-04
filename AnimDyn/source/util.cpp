#include "util.h"

std::tuple<std::string, std::string> getDirAndName(std::string fullPath)
{
  int i=0;
  for(i=fullPath.size()-1; i>=0; i--)
  {
    if(fullPath[i] == '/' || fullPath[i] == '\\')
      break;
  }
  return { fullPath.substr(0, i+1), fullPath.substr(i+1, fullPath.size() - (i+1) - 5) };
}
