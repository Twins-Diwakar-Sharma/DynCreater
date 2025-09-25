#ifndef H_DYN_CONVERTOR_H
#define H_DYN_CONVERTOR_H

#include <string>
#include "Mathril.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>


#define TYPE_BYTE 5120
#define TYPE_UNSIGNED_BYTE 5121
#define TYPE_SHORT 5122 
#define TYPE_UNSIGNED_SHORT 5123
#define TYPE_UNSIGNED_INT 5125
#define TYPE_FLOAT 5126

namespace dyn 
{
  std::tuple<std::string, std::string> getDirAndName(std::string fullPath);
  std::string convert(std::string fullPath);
}

#endif
