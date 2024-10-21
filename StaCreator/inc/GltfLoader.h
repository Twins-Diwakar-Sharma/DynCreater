#ifndef H_GLTF_LOADER_H
#define H_GLTF_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>


#define TYPE_BYTE 5120
#define TYPE_UNSIGNED_BYTE 5121
#define TYPE_SHORT 5122 
#define TYPE_UNSIGNED_SHORT 5123
#define TYPE_UNSIGNED_INT 5125
#define TYPE_FLOAT 5126

void convertGLTF(std::string gltfPath, std::string binPath, std::string outPath);
std::string getStringAfterWord(std::ifstream& infile, std::string word);

#endif
