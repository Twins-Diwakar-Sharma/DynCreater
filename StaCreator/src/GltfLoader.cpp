#include "GltfLoader.h"

void convertGLTF(std::string gltfPath, std::string binPath, std::string outPath)
{
  std::map<int, size_t> byteSizeMap;
  byteSizeMap[TYPE_BYTE] = sizeof(char*);
  byteSizeMap[TYPE_UNSIGNED_BYTE] = sizeof(unsigned char*);
  byteSizeMap[TYPE_SHORT] = sizeof(short);
  byteSizeMap[TYPE_UNSIGNED_SHORT] = sizeof(unsigned short);
  byteSizeMap[TYPE_UNSIGNED_INT] = sizeof(unsigned int);
  byteSizeMap[TYPE_FLOAT] = sizeof(float);
  std::ifstream infile(gltfPath);
  std::string line;
  size_t found;
  while(std::getline(infile, line))
  {
    found = line.find("accessors"); 
    if(found != std::string::npos)
        break;
  }
  
  int bufferNo[4] = {0};
  size_t bufferCompByte[4] = {0};
  int bufferType[4] = {3, 3, 2, 1};
  int bufferCount[4] = {0};
  int byteLength[4] = {0};
  int byteOffset[4] = {0};
  for(int i=0; i<4; i++)
  {
    bufferNo[i] = std::stoi(getStringAfterWord(infile, "bufferView"));
    int compType = std::stoi(getStringAfterWord(infile, "componentType"));
    bufferCompByte[i] = byteSizeMap[compType];
    bufferCount[i] = std::stoi(getStringAfterWord(infile, "count"));
  }


  while(std::getline(infile, line))
  {
    found = line.find("bufferViews"); 
    if(found != std::string::npos)
        break;
  }

  for(int i=0; i<4; i++)
  {
    byteLength[i] = std::stoi(getStringAfterWord(infile, "byteLength"));
    byteOffset[i] = std::stoi(getStringAfterWord(infile, "byteOffset"));
  }

  infile.close();

  infile.open(binPath, std::ios::in | std::ios::binary);
  std::vector<float> pos(bufferCount[0] * bufferType[0], 0);
  for (int i=0; i<pos.size(); i++)
  {
    infile.read((char*) &(pos[i]), (bufferCompByte[0]));
  }
  std::vector<float> norm(bufferCount[1] * bufferType[1], 0);
  for (int i=0; i<norm.size(); i++)
  {
    infile.read((char*) &(norm[i]), (bufferCompByte[1]));
  }
  std::vector<float> tex(bufferCount[2] * bufferType[2], 0);
  for (int i=0; i<tex.size(); i++)
  {
    infile.read((char*) &(tex[i]), (bufferCompByte[2]));
  }
  std::vector<unsigned int> indices(bufferCount[3] * bufferType[3], 0);
  for (int i=0; i<indices.size(); i++)
  {
    infile.read((char*) &(indices[i]), (bufferCompByte[3]));
  }

  infile.close();

  std::vector<float> vertexData(pos.size() + norm.size() + tex.size());
  for(int i=0; i<bufferCount[0]; i++)
  {
    vertexData[i*8 + 0] = pos[i*3 + 0];
    vertexData[i*8 + 1] = pos[i*3 + 1];
    vertexData[i*8 + 2] = pos[i*3 + 2];

    vertexData[i*8 + 3] = tex[i*3 + 0];
    vertexData[i*8 + 4] = tex[i*3 + 1];

    vertexData[i*8 + 5] = norm[i*3 + 0];
    vertexData[i*8 + 6] = norm[i*3 + 1];
    vertexData[i*8 + 7] = norm[i*3 + 2];
  }

  std::ofstream outfile(outPath, std::ios::out | std::ios::binary);
  
  int size = vertexData.size();
  outfile.write((char*) &size, sizeof(int));
  for (int i = 0; i < vertexData.size(); i++)
  {
    outfile.write((char*) &(vertexData[i]), sizeof(float));
  }
  size = indices.size();
  outfile.write((char*) &size, sizeof(int));
  for (int i = 0; i < indices.size(); i++)
  {
    outfile.write((char*) &(indices[i]), sizeof(unsigned int));
  }
  outfile.close();
}


std::string getStringAfterWord(std::ifstream& infile, std::string word)
{
  std::string line = "";
  size_t found;
    do{
      std::getline(infile, line);
      found = line.find(word);
    }while(found == std::string::npos);
    size_t startPos = found + word.size() + 2;
    found = line.find(",");
    size_t length = found != std::string::npos ? line.size()-1 : line.size();
    return line.substr(startPos, length - startPos);
}

