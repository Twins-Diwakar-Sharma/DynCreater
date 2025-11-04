#ifndef JALLA_ANIMATED
#define JALLA_ANIMATED

#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "glad/glad.h"
#include <iostream>
#include <map>
#include "Mathril.h"

struct Skeleton
{
  std::map<std::string, unsigned int> jointIndex;
  std::vector<std::vector<unsigned int>> jointTree;
  std::vector<int> jointParent;
};


struct JointData
{
  Vec3 translation;
  Quat rotation;
  Vec3 scale;

  JointData();
  JointData(const JointData&);
  JointData(JointData&&);
  ~JointData();
};

class MeshAnimated : public Mesh
{
protected:
	void generate(std::vector<float>&, std::vector<unsigned int>&); // creates buffers and assigns sizeOfIndices
	
public:
	MeshAnimated();
	MeshAnimated(std::string);
	~MeshAnimated();
	void bind();
	void unbind();
	int size();

  Skeleton skeleton;
  std::vector<JointData> invBind;
  std::vector<Mat4> invBindMat;
  std::map<std::string, std::vector<std::vector<JointData>>> animation;
  std::vector<std::string> indexName;
  static int max_joints; // 16 defined in cpp
};

#endif
