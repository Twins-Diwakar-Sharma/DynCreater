#include "ObjectAnimated.h"

void ObjectAnimated::syncTransform()
{

	float conv = M_PI / 180.0f;
	float radx = rot[0]*conv , rady = rot[1]*conv , radz = rot[2]*conv ;
	float cx = std::cos(radx), sx = std::sin(radx);
	float cy = std::cos(rady), sy = std::sin(rady);
	float cz = std::cos(radz), sz = std::sin(radz);

	transform[0][0] = (cy * cz) * sca[0];							transform[0][1] = (-sz * cy) * sca[1];								transform[0][2] = sy * sca[2];					transform[0][3] = pos[0];
	transform[1][0] = (cx * sz + cz * sy * sx) * sca[0];			transform[1][1] = (cz * cx - sx * sz * sy) * sca[1];				transform[1][2] = -cy * sx * sca[2];			transform[1][3] = pos[1];
	transform[2][0] = (sz * sx - cx * cz * sy) * sca[0];			transform[2][1] = (cz * sx + sz * sy * cx) * sca[1];				transform[2][2] = cx * cy * sca[2];				transform[2][3] = pos[2];
	transform[3][0] = 0;											transform[3][1] = 0;												transform[3][2] = 0;							transform[3][3] = 1;

}

ObjectAnimated::ObjectAnimated(MeshAnimated& mesh, Texture& tex) : mesh(mesh), tex(tex), currentFrameBones(mesh.skeleton.jointTree.size()), jointMatrices(mesh.skeleton.jointTree.size())
{
	for (int i = 0; i < 3; i++)
	{
		pos[i] = 0.0f;
		rot[i] = 0.0f;
		sca[i] = 1.0f;
	}
	syncTransform();
}

ObjectAnimated::~ObjectAnimated()
{
}

ObjectAnimated::ObjectAnimated(ObjectAnimated& g) : mesh(g.mesh), tex(g.tex), currentFrameBones(mesh.skeleton.jointTree.size()), jointMatrices(mesh.skeleton.jointTree.size())
{
	pos = g.pos;
	rot = g.rot;
	sca = g.sca;
	transform = g.transform;
}

ObjectAnimated::ObjectAnimated(ObjectAnimated&& g) : mesh(g.mesh), tex(g.tex), currentFrameBones(mesh.skeleton.jointTree.size()), jointMatrices(mesh.skeleton.jointTree.size())
{
	pos = std::move(g.pos);
	rot = std::move(g.rot);
	sca = std::move(g.sca);
	transform = std::move(g.transform);
}

void ObjectAnimated::bind()
{
	mesh.bind();
}

void ObjectAnimated::unbind()
{
	mesh.unbind();
}

Texture& ObjectAnimated::getTexture()
{
	return tex;
}

MeshAnimated& ObjectAnimated::getMesh()
{
	return mesh;
}

int ObjectAnimated::size()
{
	return mesh.size();
}

unsigned int ObjectAnimated::getTextureId()
{
	return tex.getTextureId();
}

void ObjectAnimated::translate(float dx, float dy, float dz)
{
	pos[0] += dx;	pos[1] += dy;	pos[2] += dz;
	for (int i = 0; i < 3; i++)
		transform[i][3] = pos[i];
}

void ObjectAnimated::rotate(float dx, float dy, float dz)
{
	rot[0] += dx;	rot[1] += dy;	rot[2] += dz;
	for (int i = 0; i < 3; i++)
	{
		if (rot[i] > 360.0f)
			rot[i] -= 360.0f;
	}
	syncTransform();
}

void ObjectAnimated::scale(float dx, float dy, float dz)
{
	sca[0] += dx;	sca[1] += dy;	sca[2] += dz;
	syncTransform();
}

void ObjectAnimated::setPosition(float x, float y, float z)
{
	pos[0] = x;	 pos[1] = y;	pos[2] = z;
	for (int i = 0; i < 3; i++)
		transform[i][3] = pos[i];
}

void ObjectAnimated::setRotation(float x, float y, float z)
{
	rot[0] = x;	rot[1] = y;	rot[2] = z;
	syncTransform();
}

void ObjectAnimated::setScale(float x, float y, float z)
{
	sca[0] = x;	sca[1] = y;	 sca[2] = z;
	syncTransform();
}

Mat4& ObjectAnimated::getTransform()
{
	return transform;
}

void ObjectAnimated::setAnimation(std::string animName)
{
  currentAnimName = animName; 
  std::cout << "showing: " << animName << std::endl;
}

void ObjectAnimated::setAnimation(int i)
{
  currentAnimName = mesh.indexName[i]; 
  std::cout << "showing: " << currentAnimName << std::endl;
}

void ObjectAnimated::animateOld() // need matrices for non uniform scale -- still does not work
{

  std::vector<std::vector<JointData>>& animData = mesh.animation[currentAnimName];  
  int nextFrameIndex = (currentFrameIndex + 1) % (animData.size());
  // local transforms
  for(unsigned int jointIndex=0; jointIndex < mesh.skeleton.jointTree.size(); jointIndex++)
  {
    for(int comp=0; comp<3; comp++) 
      currentFrameBones[jointIndex].scale[comp] = (1.0 - animTime)*animData[currentFrameIndex][jointIndex].scale[comp] + animTime*animData[nextFrameIndex][jointIndex].scale[comp];
    currentFrameBones[jointIndex].translation = (1.0 - animTime)*animData[currentFrameIndex][jointIndex].translation + animTime*animData[nextFrameIndex][jointIndex].translation;
    currentFrameBones[jointIndex].rotation = slerp(animData[currentFrameIndex][jointIndex].rotation, animData[nextFrameIndex][jointIndex].rotation, animTime);
  }
  
  //  (T2, R2, S2) * (T1, R1, S1) = T2 + R2S2*(T1 + R1S1*P) = (T2 + R2S2T1) + R2R1S2S1*P
  for(unsigned int jointIndex=0; jointIndex < mesh.skeleton.jointTree.size(); jointIndex++)
  {
    int parentIndex = mesh.skeleton.jointParent[jointIndex];
    if(parentIndex < 0)
      continue;
    Quat translateQuat(0,currentFrameBones[jointIndex].translation[0],
                        currentFrameBones[jointIndex].translation[1],
                        currentFrameBones[jointIndex].translation[2]);
    translateQuat = (currentFrameBones[parentIndex].rotation)*(translateQuat)*(~currentFrameBones[parentIndex].rotation);
    currentFrameBones[jointIndex].translation = currentFrameBones[parentIndex].translation + Vec3(translateQuat[1], translateQuat[2], translateQuat[3]);
    currentFrameBones[jointIndex].rotation = currentFrameBones[parentIndex].rotation*currentFrameBones[jointIndex].rotation;
  }
  
  // translation first this time
  // (T2, R2, S2)*(R1, S1, T1) = T2 + R2S2*( R1S1*(T1 + P) ) = T2 + R2R1S2S1T1 + R2R1S2S1*P
  for(unsigned int jointIndex=0; jointIndex < mesh.skeleton.jointTree.size(); jointIndex++)
  {
    Quat translateQuat(0, mesh.invBind[jointIndex].translation[0],
                      mesh.invBind[jointIndex].translation[1],
                      mesh.invBind[jointIndex].translation[2]);

    Quat rot = currentFrameBones[jointIndex].rotation * mesh.invBind[jointIndex].rotation; // R2*R1
    translateQuat = rot * translateQuat * ~rot;
    currentFrameBones[jointIndex].translation = currentFrameBones[jointIndex].translation + Vec3(translateQuat[1], translateQuat[2], translateQuat[3]);
    currentFrameBones[jointIndex].rotation = rot;
  }



  animTime += animTimeDelta;
  if(animTime >= 1.0)
  {
    animTime = 0.0;
    currentFrameIndex = (currentFrameIndex + 1) % (animData.size());
  }
}

void ObjectAnimated::animateNext() 
{
  currentFrameIndex = 0;
  animNum = (animNum + 1) % (mesh.animation.size());
  setAnimation(animNum);
}

void ObjectAnimated::animate()
{
  std::vector<std::vector<JointData>>& animData = mesh.animation[currentAnimName];  
  int nextFrameIndex = (currentFrameIndex + 1) % (animData.size());
  // local transforms
  for(unsigned int jointIndex=0; jointIndex < mesh.skeleton.jointTree.size(); jointIndex++)
  {
    for(int comp=0; comp<3; comp++) 
      currentFrameBones[jointIndex].scale[comp] = (1.0 - animTime)*animData[currentFrameIndex][jointIndex].scale[comp] + animTime*animData[nextFrameIndex][jointIndex].scale[comp];
    currentFrameBones[jointIndex].translation = (1.0 - animTime)*animData[currentFrameIndex][jointIndex].translation + animTime*animData[nextFrameIndex][jointIndex].translation;
    currentFrameBones[jointIndex].rotation = slerp(animData[currentFrameIndex][jointIndex].rotation, animData[nextFrameIndex][jointIndex].rotation, animTime);
    
    Mat4 scaleMat(currentFrameBones[jointIndex].scale[0], 0, 0, 0,
                  0, currentFrameBones[jointIndex].scale[1], 0, 0,
                  0, 0, currentFrameBones[jointIndex].scale[2], 0,
                  0, 0, 0, 1);
    Mat4 rotMat = (Mat4)(currentFrameBones[jointIndex].rotation);

    jointMatrices[jointIndex] = rotMat * scaleMat;
    jointMatrices[jointIndex][0][3] = currentFrameBones[jointIndex].translation[0];
    jointMatrices[jointIndex][1][3] = currentFrameBones[jointIndex].translation[1];
    jointMatrices[jointIndex][2][3] = currentFrameBones[jointIndex].translation[2];

  }

  // multiply parent transform
  for(unsigned int jointIndex=0; jointIndex < mesh.skeleton.jointTree.size(); jointIndex++)
  {
    int parentIndex = mesh.skeleton.jointParent[jointIndex];
    if(parentIndex < 0)
      continue;
    jointMatrices[jointIndex] = jointMatrices[parentIndex] * jointMatrices[jointIndex];
  }

  // invBindMatrices 
  for(unsigned int jointIndex=0; jointIndex < mesh.skeleton.jointTree.size(); jointIndex++)
  {
    jointMatrices[jointIndex] = jointMatrices[jointIndex] * mesh.invBindMat[jointIndex];
  }


  animTime += animTimeDelta;
  if(animTime >= 1.0)
  {
    animTime = 0.0;
    currentFrameIndex = (currentFrameIndex + 1) % (animData.size());
  }

}

