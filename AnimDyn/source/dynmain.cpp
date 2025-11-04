
// Remember:
// During animation:
//  0> select all bones, add k for adding keyframe
//  1> set interpolation to linear (setting cubic will add unecessary data)
// 
// During export:
//  0> select gtlf + bin
//  1> unselect sampling rate (very important, otherwise multiple keyframes)
//
//
//

#include "util.h"
#include <vector>
#include <string>
#include <fstream>
#include "Mathril.h"
#include "json.hpp"
#include <cstdlib>
#include <map>

struct JointData
{
  Vec3 translation;
  Quat rotation;
  Vec3 scale;

  JointData() : translation(0,0,0), rotation(1,0,0,0), scale(1,1,1)
  {}
};

std::string createDyn(std::string gltfPath);

int main(int argc, char** argv)
{

  std::cout << "hello from dyn " << std::endl; 

  if(argc != 2)
  {
    std::cerr << "Incorrect arguements, correct usage: " << std::endl;
    std::cerr << "DynAnim /path/to/gltf/file " <<std::endl;
    return -1;
  }
  std::string fullPath = std::string(argv[1]);
  std::string outputPath = createDyn(fullPath);

  std::string command = std::string("viewDyn.exe ") + outputPath;
  system(command.c_str());

}

std::string createDyn(std::string gltfPath)
{
  std::map<unsigned int, size_t> componentTypeMap = {
    {5120, sizeof(char)},
    {5121, sizeof(unsigned char)},
    {5122, sizeof(short)},
    {5123, sizeof(unsigned short)},
    {5125, sizeof(unsigned int)},
    {5126, sizeof(float)}
  };

  std::map<std::string, unsigned int> typeSizeMap = {
    {"VEC3", 3},
    {"VEC2", 2},
    {"VEC4", 3},
    {"MAT2", 4},
    {"SCALAR", 1},
    {"MAT3", 9},
    {"MAT4", 16}
  };


  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  ////////// ************ READ **************  /////////////
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////

  auto [dir, name] = getDirAndName(gltfPath);  
  nlohmann::json data = nlohmann::json::parse(std::ifstream(gltfPath));
  std::ifstream gltfBin(dir + name + ".bin", std::ios::in | std::ios::binary);


  unsigned int posAttribIndex = data["meshes"][0]["primitives"][0]["attributes"]["POSITION"];
  unsigned int normAttribIndex = data["meshes"][0]["primitives"][0]["attributes"]["NORMAL"];
  unsigned int texAttribIndex = data["meshes"][0]["primitives"][0]["attributes"]["TEXCOORD_0"];
  unsigned int jointsAttribIndex = data["meshes"][0]["primitives"][0]["attributes"]["JOINTS_0"];
  unsigned int weightsAttribIndex = data["meshes"][0]["primitives"][0]["attributes"]["WEIGHTS_0"];
  unsigned int indicesAttribIndex = data["meshes"][0]["primitives"][0]["indices"];

  unsigned int bufferViewIndex=0, dataCount=0, dataCompType=0, byteOffset=0, byteLength=0;

  // pos
  bufferViewIndex = data["accessors"][posAttribIndex]["bufferView"];
  dataCompType = data["accessors"][posAttribIndex]["componentType"];
  dataCount = data["accessors"][posAttribIndex]["count"];
  std::vector<float> posBuffer(dataCount * 3);
  byteOffset = data["bufferViews"][bufferViewIndex]["byteOffset"];
  byteLength = data["bufferViews"][bufferViewIndex]["byteLength"];
  gltfBin.seekg(byteOffset);
  gltfBin.read( (char*)(posBuffer.data()), byteLength );

  // norm
  bufferViewIndex = data["accessors"][normAttribIndex]["bufferView"];
  dataCompType = data["accessors"][normAttribIndex]["componentType"];
  dataCount = data["accessors"][normAttribIndex]["count"];
  std::vector<float> normBuffer(dataCount * 3);
  byteOffset = data["bufferViews"][bufferViewIndex]["byteOffset"];
  byteLength = data["bufferViews"][bufferViewIndex]["byteLength"];
  gltfBin.seekg(byteOffset);
  gltfBin.read( (char*)(normBuffer.data()), byteLength );

  // tex
  bufferViewIndex = data["accessors"][texAttribIndex]["bufferView"];
  dataCompType = data["accessors"][texAttribIndex]["componentType"];
  dataCount = data["accessors"][texAttribIndex]["count"];
  std::vector<float> texBuffer(dataCount * 2);
  byteOffset = data["bufferViews"][bufferViewIndex]["byteOffset"];
  byteLength = data["bufferViews"][bufferViewIndex]["byteLength"];
  gltfBin.seekg(byteOffset);
  gltfBin.read( (char*)(texBuffer.data()), byteLength );

  // joints
  bufferViewIndex = data["accessors"][jointsAttribIndex]["bufferView"];
  dataCompType = data["accessors"][jointsAttribIndex]["componentType"];
  dataCount = data["accessors"][jointsAttribIndex]["count"];
  std::vector<unsigned char> jointsBuffer(dataCount * 4);
  byteOffset = data["bufferViews"][bufferViewIndex]["byteOffset"];
  byteLength = data["bufferViews"][bufferViewIndex]["byteLength"];
  gltfBin.seekg(byteOffset);
  gltfBin.read( (char*)(jointsBuffer.data()), byteLength );

  // weights
  bufferViewIndex = data["accessors"][weightsAttribIndex]["bufferView"];
  dataCompType = data["accessors"][weightsAttribIndex]["componentType"];
  dataCount = data["accessors"][weightsAttribIndex]["count"];
  std::vector<float> weightsBuffer(dataCount * 4);
  byteOffset = data["bufferViews"][bufferViewIndex]["byteOffset"];
  byteLength = data["bufferViews"][bufferViewIndex]["byteLength"];
  gltfBin.seekg(byteOffset);
  gltfBin.read( (char*)(weightsBuffer.data()), byteLength );

  // indices
  bufferViewIndex = data["accessors"][indicesAttribIndex]["bufferView"];
  dataCompType = data["accessors"][indicesAttribIndex]["componentType"];
  dataCount = data["accessors"][indicesAttribIndex]["count"];
  std::vector<unsigned short> indicesBuffer(dataCount);
  byteOffset = data["bufferViews"][bufferViewIndex]["byteOffset"];
  byteLength = data["bufferViews"][bufferViewIndex]["byteLength"];
  gltfBin.seekg(byteOffset);
  gltfBin.read( (char*)(indicesBuffer.data()), byteLength );

  // skeleton : jointIndex to nodeIndex
  unsigned int numBones = data["skins"][0]["joints"].size();
  std::map<unsigned int, unsigned int> jointToNode;
  std::map<unsigned int, unsigned int> nodeToJoint;
  std::map<std::string, unsigned int> nameToJoint;
  std::map<unsigned int, std::string> jointToName;
  for(unsigned int b=0; b<numBones; b++)
  {
    jointToNode[b] = data["skins"][0]["joints"][b];
    nodeToJoint[jointToNode[b]] = b;
    jointToName[b] = data["nodes"][jointToNode[b]]["name"];
    nameToJoint[jointToName[b]] = b;

    std::cout << b << " --> " << jointToName[b] << std::endl;
  }

  // skeleton : inverseBindMatrices
  unsigned int accessorIndex = data["skins"][0]["inverseBindMatrices"];
  bufferViewIndex = data["accessors"][accessorIndex]["bufferView"];
  dataCount = data["accessors"][accessorIndex]["count"];
  if(dataCount != numBones)
  {
    std::cerr << "numBones != inverseBindMatrices.count" << std::endl;
    gltfBin.close();
    return "error";
  }
  byteOffset = data["bufferViews"][bufferViewIndex]["byteOffset"];
  byteLength = data["bufferViews"][bufferViewIndex]["byteLength"];
  std::vector<float> invBindMatricesBuffer(dataCount * 16);
  gltfBin.seekg(byteOffset);
  gltfBin.read( (char*)(invBindMatricesBuffer.data()), byteLength ); // column major
  std::vector<Mat4> invBindMatrices(dataCount);
  for(unsigned int m=0; m<dataCount; m++)
  {
    for(unsigned int col=0; col<4; col++)
    {
      for(unsigned int row=0; row<4; row++)
      {
        unsigned int stride = m * 16;
        unsigned int index = col*4 + row;
        invBindMatrices[m][row][col] = invBindMatricesBuffer[ stride + index ];
      }
    }
  }

  // skeleton : inverse bind joint data
  std::vector<JointData> invBindJointData(dataCount);
  for(unsigned int m=0; m<dataCount; m++)
  {
    invBindJointData[m].translation = Vec3(invBindMatrices[m][0][3], invBindMatrices[m][1][3], invBindMatrices[m][2][3]);
    invBindJointData[m].scale[0] = (float)(Vec3(invBindMatrices[m][0][0], invBindMatrices[m][1][0], invBindMatrices[m][2][0]));
    invBindJointData[m].scale[1] = (float)(Vec3(invBindMatrices[m][0][1], invBindMatrices[m][1][1], invBindMatrices[m][2][1]));
    invBindJointData[m].scale[2] = (float)(Vec3(invBindMatrices[m][0][2], invBindMatrices[m][1][2], invBindMatrices[m][2][2]));
    Mat3 rotMat(invBindMatrices[m][0][0]/invBindJointData[m].scale[0], invBindMatrices[m][0][1]/invBindJointData[m].scale[1], invBindMatrices[m][0][2]/invBindJointData[m].scale[2],
                invBindMatrices[m][1][0]/invBindJointData[m].scale[0], invBindMatrices[m][1][1]/invBindJointData[m].scale[1], invBindMatrices[m][1][2]/invBindJointData[m].scale[2],
                invBindMatrices[m][2][0]/invBindJointData[m].scale[0], invBindMatrices[m][2][1]/invBindJointData[m].scale[1], invBindMatrices[m][2][2]/invBindJointData[m].scale[2]);
    invBindJointData[m].rotation = (Quat)(rotMat);

      std::cout << " %%%%%%%  " << m << "  %%%%%%%" << std::endl;
      std::cout << invBindJointData[m].translation << " #--# " << invBindJointData[m].rotation 
                << " #--# " << invBindJointData[m].scale << std::endl;
  }

  unsigned int numAnims = data["animations"].size();
  std::cout << "Total animations = " << numAnims << std::endl;
  std::vector<unsigned int> numKeyFrames(numAnims);
  std::vector<std::string> animNames(numAnims);
  std::vector<std::vector<std::vector<JointData>>> animations;
  std::cout << "Animation name : no of keyframes" << std::endl;
  for(unsigned int ani=0; ani<numAnims; ani++)
  {
    std::string name = data["animations"][ani]["name"];
    animNames[ani] = name;

    unsigned int inputAccessorIndex = data["animations"][ani]["samplers"][0]["input"];  
    numKeyFrames[ani] = data["accessors"][inputAccessorIndex]["count"];
    animations.push_back(std::vector<std::vector<JointData>>(
      numKeyFrames[ani], std::vector<JointData>(numBones)
    ));
    
    std::cout << name << " : " << numKeyFrames[ani] << std::endl;
  }
  
  for(unsigned int ani=0; ani<numAnims; ani++)
  {
    std::cout << animNames[ani] << std::endl;
    unsigned int numChannels = data["animations"][ani]["channels"].size();
    for(unsigned int s=0; s<numChannels; s++)
    {
      unsigned int nodeIndex = data["animations"][ani]["channels"][s]["target"]["node"];
      std::string motionPath = data["animations"][ani]["channels"][s]["target"]["path"];
      unsigned int samplerIndex = data["animations"][ani]["channels"][s]["sampler"];
      unsigned int accessorIndex = data["animations"][ani]["samplers"][samplerIndex]["output"];
      unsigned int dataCount = data["accessors"][accessorIndex]["count"]; 
      unsigned int bufferViewIndex = data["accessors"][accessorIndex]["bufferView"];
      unsigned int byteOffset = data["bufferViews"][bufferViewIndex]["byteOffset"]; 
      unsigned int byteLength = data["bufferViews"][bufferViewIndex]["byteLength"];
      if(dataCount != numKeyFrames[ani])
      {
        std::cerr << "numKeyFrames for " << ani << "th animation does not matches dataCount for NodeIndex " << nodeIndex << " and motion type " << motionPath << std::endl;
      }
      unsigned int nComp = 0;
      unsigned int compType = 0; // 0 translation, 1 rotation, 2 scale
      if(motionPath.compare("translation") == 0)
      {
        compType = 0;
        nComp = 3;
      }
      if(motionPath.compare("rotation") == 0)
      {
        compType = 1;
        nComp = 4;
      }
      if(motionPath.compare("scale") == 0)
      {
        compType = 2;
        nComp = 3;
      }
      std::vector<float> dataBuffer(dataCount * nComp);
      gltfBin.seekg(byteOffset);
      gltfBin.read( (char*)(dataBuffer.data() ), byteLength ); 

      unsigned int jointIndex = nodeToJoint[nodeIndex];
      for(unsigned int kf=0; kf<dataCount; kf++) // dataCount should be equal to keyframe
      {
        JointData& jd = animations[ani][kf][jointIndex];
        if(compType == 1)
        {
          jd.rotation[1] = dataBuffer[kf * nComp + 0];
          jd.rotation[2] = dataBuffer[kf * nComp + 1];
          jd.rotation[3] = dataBuffer[kf * nComp + 2];
          jd.rotation[0] = dataBuffer[kf * nComp + 3];
        }
        else
        {
          Vec3 motion(0,0,0); 
          motion[0] = dataBuffer[kf * nComp + 0];
          motion[1] = dataBuffer[kf * nComp + 1];
          motion[2] = dataBuffer[kf * nComp + 2];
          if(compType == 0)
          {
            jd.translation = motion;
          }
          else if(compType == 2)
          {
            jd.scale = motion;
          }
        }

      }
    }
  }

  gltfBin.close();

  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  ////////// ************ WRITE **************  ////////////
  //////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////
  std::string outputPath(dir + name + ".dyn");
  std::ofstream dynBin(outputPath, std::ios::out | std::ios::binary);

  // interleaved data
  dataCount = data["accessors"][posAttribIndex]["count"];
  unsigned int stride = 3 + 2 + 3 + 4 + 4;
  std::vector<float> interleavedData(dataCount * stride);
  for(unsigned int i = 0; i<dataCount; i++)
  {
    // scale, rotate, translate with globalScale, globalRot, globalPos
    interleavedData[stride*i + 0] = posBuffer[3*i + 0]; 
    interleavedData[stride*i + 1] = posBuffer[3*i + 1];
    interleavedData[stride*i + 2] = posBuffer[3*i + 2];

    interleavedData[stride*i + 3] = texBuffer[2*i + 0];
    interleavedData[stride*i + 4] = 1.0f - texBuffer[2*i + 1];

    interleavedData[stride*i + 5] = normBuffer[3*i + 0]; 
    interleavedData[stride*i + 6] = normBuffer[3*i + 1];
    interleavedData[stride*i + 7] = normBuffer[3*i + 2];

    interleavedData[stride*i + 8] = (float)jointsBuffer[4*i + 0]; 
    interleavedData[stride*i + 9] = (float)jointsBuffer[4*i + 1];
    interleavedData[stride*i + 10] = (float)jointsBuffer[4*i + 2];
    interleavedData[stride*i + 11] = (float)jointsBuffer[4*i + 3];

    interleavedData[stride*i + 12] = weightsBuffer[4*i + 0]; 
    interleavedData[stride*i + 13] = weightsBuffer[4*i + 1];
    interleavedData[stride*i + 14] = weightsBuffer[4*i + 2];
    interleavedData[stride*i + 15] = weightsBuffer[4*i + 3];
  }
  dataCount = interleavedData.size();
  dynBin.write((char*) &(dataCount), sizeof(unsigned int));
  dynBin.write((char*) (interleavedData.data()), dataCount * sizeof(float));

  // indices
  std::vector<unsigned int> indicesData(indicesBuffer.size());
  for(unsigned int i=0; i<indicesBuffer.size(); i++)
  {
    indicesData[i] = (unsigned int)(indicesBuffer[i]);
  }
  dataCount = indicesData.size();
  dynBin.write((char*) &(dataCount), sizeof(unsigned int));
  dynBin.write((char*) (indicesData.data()), dataCount * sizeof(unsigned int));


  // invBindJointData
  /*
  dataCount = invBindJointData.size();
  std::vector<float> invBindJointDataBuffer(10*dataCount);
  for(unsigned int i=0; i<dataCount; i++)
  {
    invBindJointDataBuffer[10*i + 0] = invBindJointData[i].translation[0];
    invBindJointDataBuffer[10*i + 1] = invBindJointData[i].translation[1];
    invBindJointDataBuffer[10*i + 2] = invBindJointData[i].translation[2];

    invBindJointDataBuffer[10*i + 3] = invBindJointData[i].rotation[0];
    invBindJointDataBuffer[10*i + 4] = invBindJointData[i].rotation[1];
    invBindJointDataBuffer[10*i + 5] = invBindJointData[i].rotation[2];
    invBindJointDataBuffer[10*i + 6] = invBindJointData[i].rotation[3];

    invBindJointDataBuffer[10*i + 7] = invBindJointData[i].scale[0];
    invBindJointDataBuffer[10*i + 8] = invBindJointData[i].scale[1];
    invBindJointDataBuffer[10*i + 9] = invBindJointData[i].scale[2];
  }
  dynBin.write((char*) &(dataCount), sizeof(unsigned int));
  dynBin.write((char*) (invBindJointDataBuffer.data()), invBindJointDataBuffer.size() * sizeof(float));
  */ 

  // inverseBindMatrices
  dataCount = invBindMatrices.size();
  dynBin.write((char*) &(dataCount), sizeof(unsigned int));
  dynBin.write((char*) (invBindMatricesBuffer.data()), dataCount * 16 * sizeof(float));

  // skeleton tree
  std::cout << "writing skeleton structure" << std::endl;
  dynBin.write((char*) &(numBones), sizeof(unsigned int));
  for(unsigned int i=0; i<numBones; i++)
  {
    std::string name = jointToName[i];
    unsigned int sizeString = name.size();
    dynBin.write((char*) &(sizeString), sizeof(unsigned int));
    for(unsigned int ch=0; ch<sizeString; ch++)
    {
      dynBin.write((char*) &(name[ch]), sizeof(char));
    }
    unsigned int nodeIndex = jointToNode[i];
    unsigned int numChildren = data["nodes"][nodeIndex].contains("children") ? (unsigned int)(data["nodes"][nodeIndex]["children"]).size() : 0;
    dynBin.write((char*) &(numChildren), sizeof(unsigned int));
    for(unsigned int kid = 0; kid < numChildren; kid++)
    {
      unsigned int kidNodeIndex = (unsigned int)(data["nodes"][nodeIndex]["children"][kid]);
      unsigned int kidJointIndex = nodeToJoint[kidNodeIndex];
      dynBin.write((char*) &(kidJointIndex), sizeof(unsigned int));
    }
  }

  // animations
  std::cout << "writing animations matrix4" << std::endl;
  dynBin.write((char*) &(numAnims), sizeof(unsigned int));
  for(unsigned int animIndex=0; animIndex < numAnims; animIndex++)
  {    
    std::string animName = animNames[animIndex];
    unsigned int animNameSize = animName.size();
    dynBin.write((char*) &(animNameSize), sizeof(unsigned int)); 
    for(unsigned int ch=0; ch<animNameSize; ch++)
    {
      dynBin.write((char*) &(animName[ch]), sizeof(char)); 
    }
    
    unsigned int nKeys = numKeyFrames[animIndex];
    dynBin.write((char*) &(nKeys), sizeof(unsigned int));
    std::vector<float> jointDataBuffer(10);
    for(unsigned int key=0; key<nKeys; key++)
    {
      for(unsigned int bone=0; bone<numBones; bone++)
      {
        JointData& jd = animations[animIndex][key][bone];
        jointDataBuffer[0] = jd.translation[0];
        jointDataBuffer[1] = jd.translation[1];
        jointDataBuffer[2] = jd.translation[2];
        jointDataBuffer[3] = jd.rotation[0];
        jointDataBuffer[4] = jd.rotation[1];
        jointDataBuffer[5] = jd.rotation[2];
        jointDataBuffer[6] = jd.rotation[3];
        jointDataBuffer[7] = jd.scale[0];
        jointDataBuffer[8] = jd.scale[1];
        jointDataBuffer[9] = jd.scale[2];
        dynBin.write((char*)(jointDataBuffer.data()), 10 * sizeof(float));
      }
    }
  }

  dynBin.close();
  return outputPath;
}
