#include "Loader.h"

void convertObj(std::string name, int outType)
{

	std::string filepath = "inputs/" + name + ".obj";
	std::string outpath = "outputs/" + name + ".stc";

	std::vector<unsigned int> indices;
	std::vector<float> vertexData;

	std::vector<float> unorderTex, unorderNorm, fakeVertex;
	std::ifstream infile(filepath);
	std::string line, type;
	float x, y, z;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		iss >> type;
		if (type.compare("v") == 0)
		{
			iss >> x >> y >> z;
			fakeVertex.push_back(x);
			fakeVertex.push_back(y);
			fakeVertex.push_back(z);
		}
		else if (type.compare("vt") == 0)
		{
			iss >> x >> y;
			unorderTex.push_back(x);
			unorderTex.push_back(y);
		}
		else if (type.compare("vn") == 0)
		{
			iss >> x >> y >> z;
			unorderNorm.push_back(x);
			unorderNorm.push_back(y);
			unorderNorm.push_back(z);
		}
		else if (type.compare("f") == 0)
			break;

	}

	char d;
	int v1, t1, n1, v2, t2, n2, v3, t3, n3;

	MIndexTree indexTree((fakeVertex.size() / 3) + 1);

	do
	{
		std::istringstream iss(line);
		iss >> type;
		if (type.compare("f") == 0)
		{

			iss >> v1 >> d >> t1 >> d >> n1
				>> v2 >> d >> t2 >> d >> n2
				>> v3 >> d >> t3 >> d >> n3;

			// first Vertex
			int indexV1;
			bool alreadyExistsV1;
			indexV1 = indexTree.fetchIndex(v1, t1, n1, &alreadyExistsV1);
			indices.push_back(indexV1);
			if (!alreadyExistsV1)
			{
				vertexData.push_back(fakeVertex[3 * (v1 - 1)]);
				vertexData.push_back(fakeVertex[3 * (v1 - 1) + 1]);
				vertexData.push_back(fakeVertex[3 * (v1 - 1) + 2]);

				vertexData.push_back(unorderTex[2 * (t1 - 1)]);
				vertexData.push_back(unorderTex[2 * (t1 - 1) + 1]);

				vertexData.push_back(unorderNorm[3 * (n1 - 1)]);
				vertexData.push_back(unorderNorm[3 * (n1 - 1) + 1]);
				vertexData.push_back(unorderNorm[3 * (n1 - 1) + 2]);
			}

			// Second Vertex
			int indexV2;
			bool alreadyExistsV2;
			indexV2 = indexTree.fetchIndex(v2, t2, n2, &alreadyExistsV2);
			indices.push_back(indexV2);
			if (!alreadyExistsV2)
			{
				vertexData.push_back(fakeVertex[3 * (v2 - 1)]);
				vertexData.push_back(fakeVertex[3 * (v2 - 1) + 1]);
				vertexData.push_back(fakeVertex[3 * (v2 - 1) + 2]);

				vertexData.push_back(unorderTex[2 * (t2 - 1)]);
				vertexData.push_back(unorderTex[2 * (t2 - 1) + 1]);

				vertexData.push_back(unorderNorm[3 * (n2 - 1)]);
				vertexData.push_back(unorderNorm[3 * (n2 - 1) + 1]);
				vertexData.push_back(unorderNorm[3 * (n2 - 1) + 2]);
			}

			// third Vertex
			int indexV3;
			bool alreadyExistsV3;
			indexV3 = indexTree.fetchIndex(v3, t3, n3, &alreadyExistsV3);
			indices.push_back(indexV3);
			if (!alreadyExistsV3)
			{
				vertexData.push_back(fakeVertex[3 * (v3 - 1)]);
				vertexData.push_back(fakeVertex[3 * (v3 - 1) + 1]);
				vertexData.push_back(fakeVertex[3 * (v3 - 1) + 2]);

				vertexData.push_back(unorderTex[2 * (t3 - 1)]);
				vertexData.push_back(unorderTex[2 * (t3 - 1) + 1]);

				vertexData.push_back(unorderNorm[3 * (n3 - 1)]);
				vertexData.push_back(unorderNorm[3 * (n3 - 1) + 1]);
				vertexData.push_back(unorderNorm[3 * (n3 - 1) + 2]);
			}

		}

	} while (std::getline(infile, line));

	infile.close();

	if(outType == 0)
	{
		std::ofstream outfile(outpath, std::ios::out | std::ios::binary);
		
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
	else if(outType == 1)
	{
		std::ofstream outfile(outpath);
		outfile << "this.vertexData = [ ";
		for (int i = 0; i < vertexData.size(); i++)
		{
			outfile << vertexData[i] << ", ";
		}
		outfile << " ];" << std::endl;
		outfile << "this.indices = [ ";
		for (int i = 0; i < indices.size(); i++)
		{
			outfile << indices[i] << ", ";
		}
		outfile << " ];" << std::endl;
		outfile.close();
	}

	
}
