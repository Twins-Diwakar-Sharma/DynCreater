#ifndef H_DYN_CONVERTOR_H
#define H_DYN_CONVERTOR_H

#include <string>
#include <Mathril.h>
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <tuple>

namespace dyn 
{
  std::tuple<std::string, std::string> getDirAndName(std::string fullPath);
  std::string convert(std::string fullPath);
}

#endif
