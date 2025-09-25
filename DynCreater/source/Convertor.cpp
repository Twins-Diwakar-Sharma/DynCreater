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


  // 6 invBindMatrices stored in column major way in gltf
  std::vector<Mat4> invBindMatrices(data["accessors"][6]["count"]);
  for(unsigned int k=0; k<invBindMatrices.size(); k++)
  {
    for(unsigned int col=0; col<4; col++) // col major
    {
      for(unsigned int row=0; row<4; row++)
      {
        gltfBin.read((char*) &(invBindMatrices[k][row][col]), sizeof(float));
      }
    }
  }

  std::cout << data["skins"][0]["joints"].size() << std::endl;
  for(int i=0; i<data["skins"][0]["joints"].size(); i++)
  {
    int index = data["skins"][0]["joints"][i];
    std::cout << index << " : "  << data["nodes"][index]["name"] << std::endl;
  }
  
  // list names of bones in index as present in joints arrray
  // for each bone index, list the index of children, first list vector size of children, then indices


  // Animation Data: vector of keyframes
  // for each keyframe: struct of rotation, scale, translation for each bone

  return std::string("");
}
