#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
{
}

void Texture::loadDefault(std::string path)
{
  type = TEX_DEF;
	const char* cTarget = path.c_str();
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char * data = stbi_load(cTarget, &width, &height, &channels, 0);
	if (data == nullptr)
	{
		std::cerr << "Texture not found : " << path << std::endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Texture::loadHDR(std::string path)
{
  type = TEX_HDR;
	const char* cTarget = path.c_str();
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);


  stbi_ldr_to_hdr_scale(1.0f);
  stbi_ldr_to_hdr_gamma(1.0f);

  stbi_hdr_to_ldr_scale(1.0f);
  stbi_hdr_to_ldr_gamma(1.0f);

	 float * data = stbi_loadf(cTarget, &width, &height, &channels, 0);

	if (data == nullptr)
	{
		std::cerr << "Texture not found : " << path << std::endl;
	}

  std::cout << "width " << width << " height: " << height << " channels : " << channels << std::endl;

  int format = GL_RED;
  if(channels == 1)
    format = GL_RED;
  else if(channels == 3)
    format = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, format, GL_FLOAT, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

unsigned int Texture::getTextureId()
{
	return id;
}

std::string& Texture::getName()
{
	return name;
}


Texture::Texture(std::string path, Tex_Type type)
{

  if(path.size() == 0)
  {
    path = "shaders/default.png";
  }
	this->name = path;
  switch(type)
  {
    case TEX_DEF:
      loadDefault(path);
      break;
    case TEX_HDR:
      loadHDR(path);
      break;
  }
}

Texture::Texture(std::string path)
{
  if(path.size() == 0)
  {
    path = "shaders/default.png";
  }
	this->name = path;
  loadDefault(path);
}
