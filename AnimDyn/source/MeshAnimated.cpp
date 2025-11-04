#include "MeshAnimated.h"

JointData::JointData()
{}

JointData::JointData(const JointData& jd): translation(jd.translation), rotation(jd.rotation), scale(jd.scale)
{
}

JointData::JointData(JointData&& jd)
{
  translation = std::move(jd.translation);
  rotation = std::move(jd.rotation);
  scale = std::move(jd.scale);
}

JointData::~JointData()
{}


int MeshAnimated::max_joints = 16;

void MeshAnimated::generate(std::vector<float>& vertexData, std::vector<unsigned int>& indices)
{
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(5 * sizeof(float)));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);

	sizeOfIndices = indices.size();

}

MeshAnimated::MeshAnimated()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

MeshAnimated::MeshAnimated(std::string path)
{
	std::ifstream ifs(path, std::ios::binary | std::ios::in);

  // get interleaved vertex data
  unsigned int dataSize = 0;
  ifs.read((char*) &dataSize, sizeof(unsigned int));
  std::vector<float> vertexData(dataSize);
  ifs.read( (char*)(vertexData.data()), dataSize * sizeof(float));
  ifs.read((char*) &dataSize, sizeof(unsigned int));
	std::vector<unsigned int> indices(dataSize);
  ifs.read((char*)(indices.data()), dataSize * sizeof(unsigned int));
  
/*
  // inv bind JointData
  ifs.read((char*) &dataSize, sizeof(unsigned int));
  std::vector<float> tempInvBindData(dataSize * (3 + 4 + 3));
  ifs.read((char*)(tempInvBindData.data()), tempInvBindData.size() * sizeof(float));
  invBind.resize(dataSize);
  for(unsigned int i=0; i<dataSize; i++)
  {
    invBind[i].translation[0] = tempInvBindData[i*10 + 0];
    invBind[i].translation[1] = tempInvBindData[i*10 + 1];
    invBind[i].translation[2] = tempInvBindData[i*10 + 2];
    invBind[i].rotation[0] = tempInvBindData[i*10 + 3];
    invBind[i].rotation[1] = tempInvBindData[i*10 + 4];
    invBind[i].rotation[2] = tempInvBindData[i*10 + 5];
    invBind[i].rotation[3] = tempInvBindData[i*10 + 6];
    invBind[i].scale[0] = tempInvBindData[i*10 + 7];
    invBind[i].scale[1] = tempInvBindData[i*10 + 8];
    invBind[i].scale[2] = tempInvBindData[i*10 + 9];
  }
*/

  ifs.read((char*) &dataSize, sizeof(unsigned int));
  std::vector<float> invBindMatricesBuffer(dataSize*16);
  ifs.read( (char*)(invBindMatricesBuffer.data()), dataSize*16*sizeof(float)); // column major
  invBindMat.resize(dataSize);
  for(unsigned int m=0; m<dataSize; m++)
  {
    for(unsigned int col=0; col<4; col++)
    {
      for(unsigned int row=0; row<4; row++)
      {
        unsigned int stride = m * 16;
        unsigned int index = col*4 + row;
        invBindMat[m][row][col] = invBindMatricesBuffer[ stride + index ];
      }
    }
  }

  // get skeleton tree
  ifs.read((char*) &dataSize, sizeof(unsigned int));
  for(unsigned int i=0; i<dataSize; i++)
  {
    unsigned int sizeString = 0;
    ifs.read((char*) &sizeString, sizeof(unsigned int));
    std::string boneName;
    boneName.resize(sizeString);
    ifs.read((char*)(boneName.data()), sizeString * sizeof(char));
    skeleton.jointIndex[boneName] = i;
    std::cout << boneName << " -> " << i << std::endl;
    unsigned int numChildren = 0;
    ifs.read((char*) &numChildren, sizeof(unsigned int));
    skeleton.jointTree.push_back(std::vector<unsigned int>(numChildren));
    if(numChildren > 0)
    {
      ifs.read((char*) (skeleton.jointTree[i].data()), numChildren * sizeof(unsigned int));
    }
  }
  

  skeleton.jointParent.resize(dataSize, -1);
  for(int i=0; i<skeleton.jointTree.size(); i++)
  {
      std::cout << i <<  " -> " ;
    for(int j=0; j<skeleton.jointTree[i].size(); j++)
    {
      std::cout << skeleton.jointTree[i][j] << " , ";
      int childNodeIndex = skeleton.jointTree[i][j];
      skeleton.jointParent[childNodeIndex] = i;
    }
      std::cout << std::endl;
  }
  std::cout << "JOINT PARENT" << std::endl;
  for(int i=0; i<skeleton.jointParent.size(); i++)
  {
    std::cout << skeleton.jointParent[i] << std::endl;
  }
  // animations
  unsigned int nAnims = 0;
  ifs.read((char*) &nAnims, sizeof(unsigned int));
  indexName.resize(nAnims);
  for(unsigned int animIndex=0; animIndex < nAnims; animIndex++)
  {
    unsigned int sizeAnimName = 0;
    ifs.read((char*) &sizeAnimName, sizeof(unsigned int));
    std::string animName;
    animName.resize(sizeAnimName);
    ifs.read((char*)(animName.data()), sizeAnimName*sizeof(char));

    unsigned int nKeys=0;
    ifs.read((char*) &nKeys, sizeof(unsigned int));
    unsigned int sizeJoints = skeleton.jointTree.size();
    std::vector<float> temp(nKeys * sizeJoints * 10);
    ifs.read((char*)(temp.data()), temp.size() * sizeof(float));
    animation.emplace(animName, std::vector<std::vector<JointData>>(
        nKeys, std::vector<JointData>(sizeJoints)
      )
    );
  
    std::vector<std::vector<JointData>>& currentAnim = animation[animName];
    //std::cout << animName << std::endl;
    for(unsigned int key=0; key<nKeys; key++)
    {
       // std::cout << "keyFrame : " << key << std::endl;
      for(unsigned int bone=0; bone<sizeJoints; bone++)
      {
         // std::cout << "jointIndex : " << bone << std::endl;
        unsigned int index = (key*sizeJoints + bone)*(10);
        currentAnim[key][bone].translation[0] = temp[index + 0];
        currentAnim[key][bone].translation[1] = temp[index + 1];
        currentAnim[key][bone].translation[2] = temp[index + 2];
        currentAnim[key][bone].rotation[0] = temp[index + 3];
        currentAnim[key][bone].rotation[1] = temp[index + 4];
        currentAnim[key][bone].rotation[2] = temp[index + 5];
        currentAnim[key][bone].rotation[3] = temp[index + 6];
        currentAnim[key][bone].scale[0] = temp[index + 7];
        currentAnim[key][bone].scale[1] = temp[index + 8];
        currentAnim[key][bone].scale[2] = temp[index + 9];

       // std::cout << currentAnim[key][bone].translation << std::endl;
        //std::cout << currentAnim[key][bone].rotation << std::endl;
       // std::cout << currentAnim[key][bone].scale << std::endl;
      }
    }
  }
  unsigned int ani=0;
  for(auto it=animation.begin(); it!=animation.end(); ++it)
  {
    indexName[ani] = it->first; 
   // std::cout << ani << " : " << it->first << std::endl;
    ani++;
  }
  ifs.close();
  generate(vertexData, indices);

}

MeshAnimated::~MeshAnimated()
{
}


void MeshAnimated::bind()
{
	glBindVertexArray(vao);
}

void MeshAnimated::unbind()
{
	glBindVertexArray(0);
}

int MeshAnimated::size()
{
	return sizeOfIndices;
}
