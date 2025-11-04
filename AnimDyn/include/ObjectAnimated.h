#ifndef H_VASTU_CHALCHITRA_H
#define H_VASTU_CHALCHITRA_H

#include "MeshAnimated.h"
#include "Mathril.h"
#include "Texture.h"

class ObjectAnimated
{
private:
	Vec3 pos,rot,sca;
	Mat4 transform;
	MeshAnimated& mesh;
	Texture& tex;
	void syncTransform();

  float animTime=0; // [0,1]
  float animTimeDelta = 0.025f;
  unsigned int currentFrameIndex = 0;
  std::string currentAnimName="idle";

  int tabWidth=0;
  int animNum=0;

public:
	ObjectAnimated(MeshAnimated& mesh, Texture& tex);
	~ObjectAnimated();
	ObjectAnimated(ObjectAnimated&);
	ObjectAnimated(ObjectAnimated&&);
	void bind();
	void unbind();
	int size();
	unsigned int getTextureId();

	void translate(float dx, float dy, float dz);
	void rotate(float dx, float dy, float dz);
	void scale(float dx, float dy, float dz);

	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

	Mat4& getTransform();

	Texture& getTexture();
	MeshAnimated& getMesh();

  std::vector<JointData> currentFrameBones;
  std::vector<Mat4> jointMatrices;
  
  void setAnimation(std::string animName);
  void setAnimation(int i);
  void animateOld();
  void animateNext();
  void animate();
};


#endif
