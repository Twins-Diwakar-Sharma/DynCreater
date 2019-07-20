#include "Animations.h"

Animations::Animations(int noOfJoints)
{
	this->noOfJoints = noOfJoints;
}

void Animations::add(std::string type, int noOfKeys)
{
	animationNameMap.insert(std::pair<std::string, int>(type, noOfAnims));
	anims.push_back(Anim());
	anims[noOfAnims].name = type;
	anims[noOfAnims].noOfKeys = noOfKeys;
	for (int i = 0; i < noOfKeys; i++)
	{
		KeyFrame key;
		for (int j = 0; j < noOfJoints; j++)
		{
			key.joints.push_back(JointData());
		}
		anims[noOfAnims].keys.push_back(key);
	}

	noOfAnims++;
}

Anim * Animations::get(std::string type)
{
	int no = animationNameMap[type];
	Anim* address = &(anims[no]);
	return address;
}

Anim * Animations::get(int i)
{
	Anim* anim = &(anims[i]);
	return anim;
}
