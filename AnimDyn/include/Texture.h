#ifndef H_T3XTUR3_H
#define H_T3XTUR3_H

#include "glad/glad.h"
#include <iostream>
#include <string>

enum Tex_Type {
  TEX_DEF, TEX_HDR
};

class Texture
{
private:
	unsigned int id;
	int width, height, channels;
	std::string name;
  void loadDefault(std::string);
  void loadHDR(std::string);
  Tex_Type type=TEX_DEF;
public:
	Texture();
  Texture(std::string, Tex_Type type);
  Texture(std::string);
	~Texture();
	unsigned int getTextureId();
	std::string& getName();
};

#endif
