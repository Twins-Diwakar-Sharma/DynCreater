#include "Convertor.h"

std::tuple<std::string, std::string> dyn::getDirAndName(std::string fullPath)
{
  int i=0;
  for(i=fullPath.size()-1; i>=0; i--)
  {
    if(fullPath[i] == '/' || fullPath[i] == '\\')
      break;
  }
  return { fullPath.substr(0, i+1), fullPath.substr(i+1, fullPath.size() - (i+1) - 5) };
}

std::string dyn::convert(std::string fullPath)
{
  std::map<int, size_t> byteSizeMap;
  byteSizeMap[TYPE_BYTE] = sizeof(char*);
  byteSizeMap[TYPE_UNSIGNED_BYTE] = sizeof(unsigned char);
  byteSizeMap[TYPE_SHORT] = sizeof(short);
  byteSizeMap[TYPE_UNSIGNED_SHORT] = sizeof(unsigned short);
  byteSizeMap[TYPE_UNSIGNED_INT] = sizeof(unsigned int);
  byteSizeMap[TYPE_FLOAT] = sizeof(float);

  auto [dir, name] = getDirAndName(fullPath);  
  nlohmann::json data = nlohmann::json::parse(std::ifstream(fullPath));
  std::ifstream gltfBin(dir + name + ".bin", std::ios::in | std::ios::binary);

  unsigned int attrib_index[6];
  unsigned int pos_index = data["meshes"][0]["primitives"][0]["attributes"]["POSITION"];
  unsigned int norm_index = data["meshes"][0]["primitives"][0]["attributes"]["NORMAL"];
  unsigned int tex_index = data["meshes"][0]["primitives"][0]["attributes"]["TEXCOORD_0"];
  unsigned int joint_index = data["meshes"][0]["primitives"][0]["attributes"]["JOINTS_0"];
  unsigned int weight_index = data["meshes"][0]["primitives"][0]["attributes"]["WEIGHTS_0"];
  unsigned int indices_index = data["meshes"][0]["primitives"][0]["indices"];

  // 0 positions
  std::vector<Vec3> position(data["accessors"][pos_index]["count"]);
  for(unsigned int i=0; i<position.size(); i++)
  {
    gltfBin.read((char*) &(position[i][0]), sizeof(float)); 
    gltfBin.read((char*) &(position[i][1]), sizeof(float)); 
    gltfBin.read((char*) &(position[i][2]), sizeof(float)); 
  }
  // 1 normals
  std::vector<Vec3> normal(data["accessors"][norm_index]["count"]);
  for(unsigned int i=0; i<normal.size(); i++)
  {
    gltfBin.read((char*) &(normal[i][0]), sizeof(float)); 
    gltfBin.read((char*) &(normal[i][1]), sizeof(float)); 
    gltfBin.read((char*) &(normal[i][2]), sizeof(float)); 
  }

  // 2 texture coordinates
  std::vector<Vec2> texCoord(data["accessors"][tex_index]["count"]);
  for(unsigned int i=0; i<texCoord.size(); i++)
  {
    gltfBin.read((char*) &(texCoord[i][0]), sizeof(float)); 
    gltfBin.read((char*) &(texCoord[i][1]), sizeof(float)); 
  }

  // 3 joints
  std::vector<unsigned char [4]> joint(data["accessors"][joint_index]["count"]);
  for(unsigned int i=0; i<joint.size(); i++)
  {
    gltfBin.read((char*) &(joint[i][0]), sizeof(unsigned char)); 
    gltfBin.read((char*) &(joint[i][1]), sizeof(unsigned char)); 
    gltfBin.read((char*) &(joint[i][2]), sizeof(unsigned char)); 
    gltfBin.read((char*) &(joint[i][3]), sizeof(unsigned char)); 
  }

  // 4 weights
  std::vector<Vec4> weight(data["accessors"][weight_index]["count"]);
  for(unsigned int i=0; i<weight.size(); i++)
  {
    gltfBin.read((char*) &(weight[i][0]), sizeof(float)); 
    gltfBin.read((char*) &(weight[i][1]), sizeof(float)); 
    gltfBin.read((char*) &(weight[i][2]), sizeof(float)); 
    gltfBin.read((char*) &(weight[i][3]), sizeof(float)); 
  }

  // 5 indices
  int indices_size = data["accessors"][5]["count"]; 
  std::vector<unsigned int> indices(indices_size);
  for(unsigned int i=0; i<indices.size(); i++)
  {
    gltfBin.read((char*) &(indices[i]), sizeof(unsigned short));
  }

/*
  // 6 invBindMatrice storex ie colum major way in gltf
  std::vector<Mat4> invBindMatrices(data["accessors"][6]["count"]);
  std::cout << "inverse bind matrices are: " << std::endl;
  for(unsigned int k=0; k<invBindMatrices.size(); k++)
  {
      std::cout << " {< " << k << " >} " << std::endl;
    for(unsigned int col=0; col<4; col++) // col major
    {
      for(unsigned int row=0; row<4; row++)
      {
        gltfBin.read((char*) &(invBindMatrices[k][row][col]), sizeof(float));
      }
    }

      std::cout << invBindMatrices[k] << std::endl;
      std::cout << std::endl;
  }
  
  // convert matrix to pos, quat, scale
  std::vector<std::tuple<Vec3,Quat,Vec3>> invBindTuple(invBindMatrices.size());
  for(unsigned int k=0; k<invBindTuple.size(); k++)
  {
    Vec3 scaleFromMat;
    scaleFromMat[0] = (float)(Vec3(invBindMatrices[k][0][0], invBindMatrices[k][1][0], invBindMatrices[k][2][0]));
    scaleFromMat[1] = (float)(Vec3(invBindMatrices[k][0][1], invBindMatrices[k][1][1], invBindMatrices[k][2][1]));
    scaleFromMat[2] = (float)(Vec3(invBindMatrices[k][0][2], invBindMatrices[k][1][2], invBindMatrices[k][2][2]));
    Vec3 posFromMat(invBindMatrices[k][0][3], invBindMatrices[k][1][3], invBindMatrices[k][2][3]);
    Mat3 rotMat(invBindMatrices[k][0][0]/scaleFromMat[0], invBindMatrices[k][0][1]/scaleFromMat[1], invBindMatrices[k][0][2]/scaleFromMat[2],
                invBindMatrices[k][1][0]/scaleFromMat[0], invBindMatrices[k][1][1]/scaleFromMat[1], invBindMatrices[k][1][2]/scaleFromMat[2],
                invBindMatrices[k][2][0]/scaleFromMat[0], invBindMatrices[k][2][1]/scaleFromMat[1], invBindMatrices[k][2][2]/scaleFromMat[2]);
    Quat rotQuat = (Quat)(rotMat);
    invBindTuple[k] = std::make_tuple(posFromMat, rotQuat, scaleFromMat);
  }
*/

  
  unsigned int totalBones = (unsigned int)(data["skins"][0]["joints"].size()); 
  std::cout << data["skins"][0]["joints"].size() << std::endl;
  std::vector<std::string> boneNameJointIndex(totalBones);
  std::vector<int> nodeToJointIndex(totalBones);
  std::vector<int> jointIndexToNode(totalBones);
  for(int i=0; i<data["skins"][0]["joints"].size(); i++)
  {
    int index = data["skins"][0]["joints"][i];
    std::cout << i << " -> " << index << " : "  << data["nodes"][index]["name"] << std::endl;
    boneNameJointIndex[i] = std::string(data["nodes"][index]["name"]);
    nodeToJointIndex[index] = i;
    jointIndexToNode[i] = index;
  }


  // pos rot scale
  std::vector<std::tuple<Vec3,Quat,Vec3>> invBindTuple(totalBones);
  for(unsigned int ji=0; ji<totalBones; ji++)
  {
    unsigned int nodeIndex = jointIndexToNode[ji];
    nlohmann::json& jointNodeData = data["nodes"][nodeIndex];
    Vec3 position(0,0,0); 
    if(jointNodeData.contains("translation"))
    {
      for(unsigned int i=0; i<3; i++)
      {
        position[i] = jointNodeData["translation"][i];
      }
      // invert position
      position = -1.0 * position;
    }
    Quat rotation(1,0,0,0);
    if(jointNodeData.contains("rotation"))
    {
      Vec4 nakliRotation(0,0,0,1);
      for(unsigned int i=0; i<4; i++)
      {
        nakliRotation[i] = jointNodeData["rotation"][i];
      }
      // invert rotation
      rotation = ~(Quat(nakliRotation[3], nakliRotation[0], nakliRotation[1], nakliRotation[2]));
    }
    Vec3 scale(1,1,1);
    if(jointNodeData.contains("scale"))
    {
      for(unsigned int i=0; i<3; i++)
      {
        scale[i] = jointNodeData["scale"][i]; 
      }
      // invert scale
      scale = Vec3(1.0/scale[0], 1.0/scale[1], 1.0/scale[2]);
    }

    invBindTuple[ji] = std::make_tuple(position, rotation, scale);
  }

  
  // list names of bones in index as present in joints arrray
  // for each bone index, list the index of children, first list vector size of children, then indices


  // Animation Data
  //  => vector of animations
  //  ==> vector of keyframes
  //  ===> for each keyframe: struct of rotation, scale, translation for each bone
  unsigned int nAnims = data["animations"].size();
  unsigned int nJoints = data["skins"][0]["joints"].size();
  std::vector<std::vector<std::vector<std::tuple<Vec3,Quat,Vec3>>>> anims(nAnims);
  for(unsigned int animIndex=0; animIndex < nAnims; animIndex++)
  {
    unsigned int accessorNo = data["animations"][animIndex]["samplers"][0]["input"];
    unsigned int bufferNo = data["accessors"][accessorNo]["bufferView"]; 
    unsigned int nKeys = data["accessors"][accessorNo]["count"]; 
    unsigned int byteOffset = data["bufferViews"][bufferNo]["byteOffset"]; 
    
    for(unsigned int keyIndex=0; keyIndex < nKeys; keyIndex++)
    {
      anims[animIndex].push_back(std::vector<std::tuple<Vec3,Quat,Vec3>>(nJoints));
    }


    for(unsigned int jointIndex=0; jointIndex < nJoints; jointIndex++)
    {
      // instead of index, we use the order as in skins[0].joints, why idk
      //unsigned int asliJointIndex = data["skins"][0]["joints"][jointIndex]; // yeh mein kyun kar rha hoon? patanhi
      unsigned int asliJointIndex = jointIndexToNode[jointIndex];
      // iterate channels, find accessor no for translation, rotation, scale, for current jointNo
      unsigned int posIndex=-1, rotIndex=-1, scaIndex=-1;
      for(unsigned int chan=0; chan<data["animations"][animIndex]["channels"].size(); chan++)
      {
        nlohmann::json& channelJson = data["animations"][animIndex]["channels"][chan];
        
        if(channelJson["target"]["node"] == asliJointIndex)
        {
          std::string channelPath = channelJson["target"]["path"];
          if(channelPath.compare("rotation") == 0)
          {
            rotIndex = (unsigned int)channelJson["sampler"];
            rotIndex = (unsigned int)data["animations"][animIndex]["samplers"][rotIndex]["output"];
          }
          else if(channelPath.compare("translation") == 0)
          {
            posIndex = (unsigned int)channelJson["sampler"];
            posIndex = (unsigned int)data["animations"][animIndex]["samplers"][posIndex]["output"];
          }
          else if(channelPath.compare("scale") == 0)
          {
            scaIndex = (unsigned int)channelJson["sampler"];
            scaIndex = (unsigned int)data["animations"][animIndex]["samplers"][scaIndex]["output"];
          }
        }    
      }

      unsigned int buffViewNo = (unsigned int)(data["accessors"][posIndex]["bufferView"]);
      byteOffset = (unsigned int) data["bufferViews"][buffViewNo]["byteOffset"];
      gltfBin.seekg(byteOffset);
      std::vector<Vec3> jointPositions(nKeys);
      for(unsigned int key=0; key<nKeys; key++)
      {
        for(int dim=0; dim<3; dim++)
        {
          float fl;
          gltfBin.read((char*)&fl, sizeof(float));
          jointPositions[key][dim] = fl;
        }
      }

      
      buffViewNo = (unsigned int)(data["accessors"][rotIndex]["bufferView"]);
      byteOffset = (unsigned int) data["bufferViews"][buffViewNo]["byteOffset"];
      gltfBin.seekg(byteOffset);
      std::vector<Quat> jointRotations(nKeys);
      // quaternions stored [x y z w]
      for(unsigned int key=0; key<nKeys; key++)
      {
        for(int dim=1; dim<4; dim++)
        {
          float fl;
          gltfBin.read((char*)&fl, sizeof(float));
          jointRotations[key][dim] = fl;
        }
        float fl;
        gltfBin.read((char*)&fl, sizeof(float));
        jointRotations[key][0] = fl;
      }


      
      buffViewNo = (unsigned int)(data["accessors"][scaIndex]["bufferView"]);
      byteOffset = (unsigned int)data["bufferViews"][buffViewNo]["byteOffset"];
      gltfBin.seekg(byteOffset);
      std::vector<Vec3> jointScales(nKeys);
      for(unsigned int key=0; key<nKeys; key++)
      {
        for(int dim=0; dim<3; dim++)
        {
          float fl;
          gltfBin.read((char*)&fl, sizeof(float));
          jointScales[key][dim] = fl;
        }
      }


      
      for(unsigned int key=0; key<nKeys; key++)
      {
        //anims[animIndex][key][asliJointIndex] = std::make_tuple(Vec3(jointPositions[key]), Quat(jointRotations[key]), Vec3(jointScales[key]));
        anims[animIndex][key][jointIndex] = std::make_tuple(Vec3(jointPositions[key]), Quat(jointRotations[key]), Vec3(jointScales[key]));
      }

    }
  }
  gltfBin.close();
  std::cout << "Gathering information done" << std::endl;
  std::cout << "Generating new file" << std::endl;
  
  unsigned int comps = 3 + 2 + 3 + 4 + 4;
  std::vector<float> interleavedData(position.size() * comps );
  for(unsigned int i=0; i<position.size(); i++)
  {
    interleavedData[i*comps + 0] = position[i][0];
    interleavedData[i*comps + 1] = position[i][1];
    interleavedData[i*comps + 2] = position[i][2];

    interleavedData[i*comps + 3] = texCoord[i][0];
    interleavedData[i*comps + 4] = 1.0f - texCoord[i][1];

    interleavedData[i*comps + 5] = normal[i][0];
    interleavedData[i*comps + 6] = normal[i][1];
    interleavedData[i*comps + 7] = normal[i][2];

    interleavedData[i*comps + 8] = joint[i][0];
    interleavedData[i*comps + 9] = joint[i][1];
    interleavedData[i*comps + 10] = joint[i][2];
    interleavedData[i*comps + 11] = joint[i][3];

    interleavedData[i*comps + 12] = weight[i][0];
    interleavedData[i*comps + 13] = weight[i][1];
    interleavedData[i*comps + 14] = weight[i][2];
    interleavedData[i*comps + 15] = weight[i][3];

  }
  
  // interleaved data
  std::cout << "writing interleaved data" << std::endl;
  std::ofstream dynBin(dir + name + ".dyn", std::ios::out | std::ios::binary);
  unsigned int interleavedDataSize = interleavedData.size();
  dynBin.write((char*) &(interleavedDataSize), sizeof(unsigned int));
  for(unsigned int i=0; i<interleavedDataSize; i++)
  {
    dynBin.write((char*) &(interleavedData[i]), sizeof(float));
  }

  // indices
  
  unsigned int indicesSize = indices_size;
  dynBin.write((char*) &(indicesSize), sizeof(unsigned int)); 
  for(unsigned int i=0; i<indicesSize; i++)
  {
    dynBin.write((char*) &(indices[i]), sizeof(unsigned int));
  }

  // inv bind quat rot pos
  unsigned int invBindSize = invBindTuple.size();
  dynBin.write((char*) &invBindSize, sizeof(unsigned int));
  std::cout << "Inverse bind Joint Data " << std::endl;
  for(unsigned int i=0; i<invBindTuple.size(); i++)
  {
    std::cout << " <{ " << i <<  " }> " << std::endl;
    Vec3 pos = std::get<0>(invBindTuple[i]);
    Quat rot = std::get<1>(invBindTuple[i]);
    Vec3 sca = std::get<2>(invBindTuple[i]);

    std::cout << "pos : " << pos << std::endl;
    std::cout << "rot : " << rot << std::endl;
    std::cout << "sca : " << sca << std::endl;

    dynBin.write((char*) &(pos[0]), sizeof(float));
    dynBin.write((char*) &(pos[1]), sizeof(float));
    dynBin.write((char*) &(pos[2]), sizeof(float));

    dynBin.write((char*) &(rot[0]), sizeof(float));
    dynBin.write((char*) &(rot[1]), sizeof(float));
    dynBin.write((char*) &(rot[2]), sizeof(float));
    dynBin.write((char*) &(rot[3]), sizeof(float));

    dynBin.write((char*) &(sca[0]), sizeof(float));
    dynBin.write((char*) &(sca[1]), sizeof(float));
    dynBin.write((char*) &(sca[2]), sizeof(float));
  }
  
  // bones information
  std::cout << "writing skeleton structure" << std::endl;
  unsigned int jointsSize = data["skins"][0]["joints"].size();
  dynBin.write((char*) &(jointsSize), sizeof(unsigned int));
  for(unsigned int i=0; i<jointsSize; i++)
  {

    // size of string
    std::string name = boneNameJointIndex[i];
    unsigned int sizeString = name.size();
    dynBin.write((char*) &(sizeString), sizeof(unsigned int));
    for(unsigned int ch=0; ch<sizeString; ch++)
    {
      dynBin.write((char*) &(name[ch]), sizeof(char));
    }
    unsigned int nodeIndex = jointIndexToNode[i];
    unsigned int numChildren = data["nodes"][nodeIndex].contains("children") ? (unsigned int)(data["nodes"][nodeIndex]["children"]).size() : 0;
    dynBin.write((char*) &(numChildren), sizeof(unsigned int));
    std::cout << name << " =:=> " ;
    for(unsigned int kid = 0; kid < numChildren; kid++)
    {
      unsigned int kidNodeIndex = (unsigned int)(data["nodes"][nodeIndex]["children"][kid]);
      unsigned int kidIndex = nodeToJointIndex[kidNodeIndex];
      dynBin.write((char*) &(kidIndex), sizeof(unsigned int));
        std::cout << boneNameJointIndex[kidIndex] << " ";
    }
      std::cout << std::endl;
  }

  
  // animations
  std::cout << "writing animations " << std::endl;
  dynBin.write((char*) &(nAnims), sizeof(unsigned int));
  for(unsigned int animIndex=0; animIndex < nAnims; animIndex++)
  {    
    std::string animName = data["animations"][animIndex]["name"];
    unsigned int animNameSize = animName.size();
    dynBin.write((char*) &(animNameSize), sizeof(unsigned int)); 
    for(unsigned int ch=0; ch<animNameSize; ch++)
    {
      dynBin.write((char*) &(animName[ch]), sizeof(char)); 
    }
    
    unsigned int nKeys = anims[animIndex].size();
    dynBin.write((char*) &(nKeys), sizeof(unsigned int));
    for(unsigned int key=0; key<nKeys; key++)
    {
      for(unsigned int bone=0; bone<jointsSize; bone++)
      {
        Vec3& pos = std::get<0>(anims[animIndex][key][bone]);
        Quat& rot = std::get<1>(anims[animIndex][key][bone]);
        Vec3& sca = std::get<2>(anims[animIndex][key][bone]);


        dynBin.write((char*) &(pos[0]), sizeof(float));
        dynBin.write((char*) &(pos[1]), sizeof(float));
        dynBin.write((char*) &(pos[2]), sizeof(float));

        dynBin.write((char*) &(rot[0]), sizeof(float));
        dynBin.write((char*) &(rot[1]), sizeof(float));
        dynBin.write((char*) &(rot[2]), sizeof(float));
        dynBin.write((char*) &(rot[3]), sizeof(float));

        dynBin.write((char*) &(sca[0]), sizeof(float));
        dynBin.write((char*) &(sca[1]), sizeof(float));
        dynBin.write((char*) &(sca[2]), sizeof(float));
      }
    }
  }
  
  dynBin.close();
  return std::string("");
}
