#ifndef CHAL_CHITRA
#define CHAL_CHITRA
	
#include <string>
#include <map>
#include <vector>
#include "Quaternion.h"

struct JointData
{
	Matrix4f local,world;

	Quaternion rotation;
	Vector3f translation;
};

struct KeyFrame
{
	std::vector<JointData> joints;
};

struct Anim
{
	std::string name;
	int noOfKeys;
	std::vector<KeyFrame> keys;
};

class Animations
{
public:
	int noOfJoints=0, noOfAnims=0;
	
	Animations(int noOfJoints);

	void add(std::string type, int noOfKeys);
	Anim* get(std::string type);
	Anim* get(int i);

private:
	std::vector<Anim> anims;
	std::map<std::string, int> animationNameMap;
};

#endif

