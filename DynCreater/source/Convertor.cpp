#include "Convertor.h"

std::tuple<std::string, std::string> dyn::getDirAndName(std::string fullPath)
{
    int i=0;
    for(i=fullPath.size()-1; i>=0; i--)
    {
      if(fullPath[i] == '/' || fullPath[i] == '\\')
        break;
    }
    return { fullPath.substr(0, i+1), fullPath.substr(i+1, fullPath.size() - (i+1) - 5) };
}

std::string dyn::convert(std::string fullPath)
{
  auto [dir, name] = getDirAndName(fullPath);  
  nlohmann::json data = nlohmann::json::parse(std::ifstream(fullPath));
  for(int i=0; i<data["nodes"].size(); i++)
  {
     std::cout <<  data["nodes"][i] << std::endl;
  }
  return "";
}
