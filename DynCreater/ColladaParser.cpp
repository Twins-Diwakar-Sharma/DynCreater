#include "ColladaParser.h"

ColladaParser::ColladaParser(std::string fileName)
{
	this->fileName = fileName;
	initialize();
}

void ColladaParser::initialize()
{
	std::string type = "ranged";
	loadAnim(type, true);  // load skeleton too

    type = "melee";
	loadAnim(type, false);	// do not load skeleton

	if (!error)
	{
		saveAnim();
		std::cout << "Successfuly created : " << fileName << ".dyn " << std::endl;
	}
	else
	{
		std::cerr << "Could not finish creating dyn, some of the required files missing" << std::endl;
	}
	system("pause");	// press any key to continue message 
}

void ColladaParser::loadAnim(std::string& type, bool skeletonSetup)
{
	std::string fileNameDae = fileName + "_" + type + ".dae";
	inFile = new std::ifstream(fileNameDae);

	if (!inFile->good())
	{
		std::cerr << "The file :: "<<  fileNameDae <<" ::  not found " << std::endl;
		error = 1;
		return;
	}

	std::string line;
	
	if (skeletonSetup)	// if default setup load everything else
	{
		// library_geometries
		std::vector<float> readVData[4];
		std::vector<int> p;
		std::vector<int> vcount, v;

		std::streampos startLoc = inFile->tellg();

		while (std::getline(*inFile, line))
		{
			if (line.find("<library_geometries>") != std::string::npos)
			{
				break;
			}
		}
		library_geometries(readVData, p);

		std::streampos geometryEndLoc = inFile->tellg();
		
		while (std::getline(*inFile, line))
		{

			if (line.find("<library_controllers>") != std::string::npos)
			{
				break;
			}
		}
		library_controllers(readVData, vcount , v);

		////////////////////////////////////////////////////////////////
				createVertexData(readVData, p, vcount, v);
		////////////////////////////////////////////////////////////////	

		while (std::getline(*inFile, line))
		{
			if (line.find("<library_visual_scenes>") != std::string::npos)
			{
				break;
			}
		}
		library_visual_scenes();
		
		//////////////////////////////////////////////////
					worldBindMatrix(rootID);
		/////////////////////////////////////////////////

		inFile->seekg(startLoc);
	}
	// if not default Setup load only animations

	while (std::getline(*inFile, line))
	{
		
		if (line.find("<library_animations>") != std::string::npos)
		{
			break;
		}
	}
	library_animations(type);
	
	/////////////////////////////////////////////////////////////////
					generateWorldMatrices(type);
	/////////////////////////////////////////////////////////////////

	inFile->close();
}

void ColladaParser::library_geometries(std::vector<float> data[], std::vector<int>& p)
{
	std::string line;
	int dataIndex = -1;


	while (std::getline(*inFile, line))
	{

		if (line.find("</library_geometries>") != std::string::npos)
			break;

		if (line.find("<float_array") != std::string::npos)
		{
			dataIndex++;
			size_t findstart, findend;
			findstart = line.find(">");
			findend = line.find("<", findstart + 1);
			line = line.substr(findstart + 1, findend - (findstart + 1));

			std::istringstream iss(line);
			float f;
			while (iss >> f)
			{
				data[dataIndex].push_back(f);
			}
		}

		if (line.find("<p>") != std::string::npos)
		{
			size_t findstart, findend;
			findstart = line.find(">");
			findend = line.find("<", findstart + 1);
			line = line.substr(findstart + 1, findend - (findstart + 1));

			std::istringstream iss(line);
			int i;
			while (iss >> i)
			{
				p.push_back(i);
			}
		}
	}
	
}

void ColladaParser::library_controllers(std::vector<float> data[], std::vector<int>& vcount, std::vector<int>& v)
{
	std::string line;


	while (getline(*inFile, line))
	{
		if (line.find("</library_controllers>") != std::string::npos)
			break;

		// bind shape Matrix per skeleton
		if (line.find("<bind_shape_matrix>") != std::string::npos)
		{
			size_t start, end;
			start = line.find(">");
			end = line.find("<", start + 1);
			line = line.substr(start + 1, end - (start + 1));

			std::istringstream iss(line);	
			int col = 0, row = 0;
			float f;
			while (iss >> f)
			{
				bindShapeMatrix[col][row] = f;
				col++;
				if (col > 3)
				{
					col = 0;
					row++;
				}
			}
		}

		// joints
		if (line.find("<Name_array") != std::string::npos)
		{
			size_t start, end;
			start = line.find(">");	end = line.find("<", start + 1);
			line = line.substr(start + 1, end - (start + 1));

			std::istringstream iss(line);
			std::string jointName;
			int jointID = 0;
			while (iss >> jointName)
			{
				this->jointMap.insert(std::pair<std::string, int>(jointName, jointID));
				jointID++;
			}
			this->noOfJoints = jointID;

			invBindMatrix = new Matrix4f[noOfJoints];
			animations = new Animations(noOfJoints);
		}

		// inverse bind shape Matrix per joint
		if (line.find("<float_array") != std::string::npos  && line.find("skin-bind_poses-array") != std::string::npos)
		{
			size_t start, end;
			start = line.find(">");	 end = line.find("<", start + 1);
			line = line.substr(start + 1, end - (start + 1));

			std::istringstream iss(line);
			float f; int row = 0, col = 0, jointID = 0;
			while (iss >> f)
			{
				invBindMatrix[jointID][col][row] = f;
				col++;
				if (col > 3)
				{
					col = 0;
					row++;
					if (row > 3)
					{
						row = 0;
						jointID++;
					}
				}
			}
		}

		// weights
		if (line.find("<float_array") != std::string::npos && line.find("skin-weights-array") != std::string::npos)
		{
			size_t start, end;
			start = line.find(">");	end = line.find("<", start + 1);
			line = line.substr(start + 1, end - (start + 1));

			std::istringstream iss(line);
			float f;
			while (iss >> f)
			{
				data[3].push_back(f);
			}
		}

		// vcount
		if (line.find("<vcount>") != std::string::npos)
		{
			size_t start, end;
			start = line.find(">");	end = line.find("<", start + 1);
			line = line.substr(start + 1, end - (start + 1));

			std::istringstream iss(line);
			int i;
			while (iss >> i)
			{
				vcount.push_back(i);
			}
		}

		if (line.find("<v>") != std::string::npos)
		{
			size_t start, end;
			start = line.find(">");	end = line.find("<", start + 1);
			line = line.substr(start + 1, end - (start + 1));

			std::istringstream iss(line);
			int i;
			while (iss >> i)
			{
				v.push_back(i);
			}
		}

	}
}

void ColladaParser::library_visual_scenes()
{
	std::string line;
	for (int s = 0; s < noOfJoints; s++)
	{
		this->heirarchy.push_back(std::vector<int>());
	}
	bindMatrix = new Matrix4f[noOfJoints];

	while (getline(*inFile, line))
	{
		if (line.find("</library_visual_scenes>") != std::string::npos)
			break;
		if (line.find("<node") != std::string::npos && line.find("id=\"Armature\"" )  && line.find("type=\"NODE\"") != std::string::npos)
		{
			getline(*inFile, line);	// matrix line
			size_t start, end;
			start = line.find(">");	end = line.find("<", start + 1);
			line = line.substr(start+1, end - (start+1));
			std::istringstream iss(line);
			float f;	int row = 0, col = 0;
			while (iss >> f)
			{
				armatureMatrix[col][row] = f;
				col++;
				if (col > 3)
				{
					col = 0;
					row++;
				}
			}

		
		}
		if (line.find("<node") != std::string::npos && line.find("type=\"JOINT\"") != std::string::npos)
		{
			recursiveHeirarchy(-1, line);
		}
	}

}

void ColladaParser::library_animations(std::string type)
{
	bool keysSet = false;
	std::string line;

	while (getline(*inFile, line))
	{
		if (line.find("</library_animations>") != std::string::npos)
		{
			break;
		}
		if (line.find("<float_array") != std::string::npos && line.find("matrix-input-array") != std::string::npos && !keysSet)
		{
			keysSet = true;
			std::string tag, id, count;
			std::istringstream in(line);
			in >> tag >> id >> count;
			size_t start = count.find("\""), end = count.find("\"", start + 1);
			count = count.substr(start + 1, end - (start + 1));
			std::istringstream in2(count);
			int k;
			in2 >> k;
			animations->add(type, k);
		}
		if (line.find("<float_array") != std::string::npos && line.find("matrix-output-array") != std::string::npos)
		{
			//find jointID
			std::istringstream searchJoint(line);
			std::string tag, id;
			searchJoint >> tag >> id;
			size_t start_ = id.find("_"), end_ = id.find("_pose_matrix");
			id = id.substr(start_+1,end_ - (start_ + 1));
			int jointID = jointMap[id];
			//matrices
			size_t start = line.find(">"), end = line.find("<", start + 1);
			line = line.substr(start + 1, end - (start+1));

			float f;	int row = 0, col = 0, keyNo = 0;
			
			std::istringstream iss(line);
			Anim* currentAnimation = animations->get(type);

			while (iss >> f)
			{
				currentAnimation->keys[keyNo].joints[jointID].local[col][row] = f;
				col++;
				if (col > 3)
				{
					col = 0;
					row++;
					if (row > 3)
					{
						row = 0;
						keyNo++;
					}
				}
			}
		}
	}
}

// ......................calculations......................
void ColladaParser::createVertexData(std::vector<float> data[], std::vector<int>& p, std::vector<int>& vcount, std::vector<int>& v)
{
	// adjust weights
	std::vector<float> w;	// new 3 weights
	std::vector<int> jid;	// corresponding 3 joints

	int cnt = 0;
	
	for (int i = 0; i < vcount.size(); i++)
	{
		if (vcount[i] <= 3)
		{
			int j = 0;
			for (; j < vcount[i]; j++)
			{
				jid.push_back(v[cnt]);	cnt++;
				w.push_back(data[3][v[cnt]]);	cnt++;
			}
			for (; j < 3; j++)
			{
				jid.push_back(-1);
				w.push_back(0);	
			}
		}
		else
		{
			std::vector<float> moreW;
			std::vector<int> moreJID;
			for (int j = 0; j < vcount[i]; j++)
			{
				moreJID.push_back(v[cnt]);	cnt++;
				moreW.push_back(data[3][v[cnt]]);	cnt++;
			}
			// find greatest 3
			int s = -1, m = -1, l = -1;	float sw = -1, mw = -1, lw = -1;
			for (int g = 0; g < vcount[i]; g++)
			{
				if (moreW[g] > lw)
				{
					sw = mw; s = m;
					mw = lw; m = l;
					lw = moreW[g];	l = g;
				}
				else if (moreW[g] > mw)
				{
					sw = mw; s = m;
					mw = moreW[g];	m = g;
				}
				else if (moreW[g] > sw)
				{
					sw = moreW[g];	s = g;
				}
			}
			jid.push_back(	moreJID[l]); jid.push_back(moreJID[m]); jid.push_back(moreJID[s]);
			Vector3f normalizedWeights(moreW[l], moreW[m], moreW[s]);
			normalizedWeights = normalizedWeights.dirtection();
			w.push_back(normalizedWeights.x()); w.push_back(normalizedWeights.y()); w.push_back(normalizedWeights.z());
		}
	}

	MIndexTree indexTree(data[0].size() / 3);

	// construct Vertex Data from p
	for (int i = 0; i < p.size(); i+=3)
	{
		int posIndex = p[i];
		int norIndex = p[i + 1];
		int texIndex = p[i + 2];

		int indexV;
		bool alreadyThere;
		indexV = indexTree.fetchIndex(posIndex, texIndex, norIndex, &alreadyThere);
		indices.push_back(indexV);


		if (!alreadyThere)
		{
			//	position
			vertexData.push_back(data[0][posIndex * 3 + 0]);
			vertexData.push_back(data[0][posIndex * 3 + 1]);
			vertexData.push_back(data[0][posIndex * 3 + 2]);

			// uv
			vertexData.push_back(data[2][texIndex * 2 + 0]);
			vertexData.push_back(data[2][texIndex * 2 + 1]);

			// normal
			vertexData.push_back(data[1][norIndex * 3 + 0]);
			vertexData.push_back(data[1][norIndex * 3 + 1]);
			vertexData.push_back(data[1][norIndex * 3 + 2]);

			// jid
			vertexData.push_back(jid[posIndex * 3 + 0]);
			vertexData.push_back(jid[posIndex * 3 + 1]);
			vertexData.push_back(jid[posIndex * 3 + 2]);

			// weights
			vertexData.push_back(w[posIndex*3 + 0]);
			vertexData.push_back(w[posIndex*3 + 1]);
			vertexData.push_back(w[posIndex*3 + 2]);
		}
	}

	
}

void ColladaParser::recursiveHeirarchy(int parentID, std::string& line)
{
	std::istringstream nodestream(line);
	std::string node, id, name, sid, type;
	nodestream >> node >> id >> name >> sid >> type;
	size_t startsid = sid.find("\""), endsid = sid.find("\"", startsid + 1);
	sid = sid.substr(startsid + 1, endsid - (startsid + 1));

	int jointID = jointMap[sid];
	if (parentID >= 0)
		heirarchy[parentID].push_back(jointID);
	else
		rootID = jointID;

	while (getline(*inFile, line))
	{
		if (line.find("</node>") != std::string::npos)
		{
			break;
		}
		if (line.find("<matrix") != std::string::npos)
		{
			size_t start = line.find(">"), end = line.find("<", start + 1);
			line = line.substr(start + 1, end - (start + 1));
			float f; int row = 0, col = 0;
			std::istringstream matrix(line);
			while (matrix >> f)
			{
				this->bindMatrix[jointID][col][row] = f;
				col++;
				if (col > 3)
				{
					col = 0;
					row++;
				}
			}
		}
		if (line.find("<node") != std::string::npos)
		{
			recursiveHeirarchy(jointID, line);
		}
	}
}

void ColladaParser::generateWorldMatrices(std::string& type)
{
	// -90 degree rotation to root
	Matrix4f correction;
	correction[1][1] = 0;	correction[1][2] = -1;
	correction[2][1] = 1;   correction[2][2] = 0;

	Anim* currentAnimation = animations->get(type);

	// joint matrices from local to world
	for (int i = 0; i < currentAnimation->noOfKeys; i++)
	{
		currentAnimation->keys[i].joints[rootID].world =  correction * currentAnimation->keys[i].joints[rootID].local;
		localToWorld(currentAnimation->keys[i],rootID);
	}

	// vertex to animationWorld
	for (int i = 0; i < currentAnimation->noOfKeys; i++)
	{
		
		for (int j = 0; j < noOfJoints; j++)
		{
			currentAnimation->keys[i].joints[j].world = (currentAnimation->keys[i].joints[j].world) * (this->invBindMatrix[j]) * (this->bindShapeMatrix);
			mathematics::convertMatrix(currentAnimation->keys[i].joints[j].world, currentAnimation->keys[i].joints[j].rotation, currentAnimation->keys[i].joints[j].translation);
		}
	}

	

}

void ColladaParser::localToWorld(KeyFrame& k, int rootID)
{
	for (int i = 0; i < heirarchy[rootID].size(); i++)
	{
		k.joints[heirarchy[rootID][i]].world =  (k.joints[rootID].world) * (k.joints[heirarchy[rootID][i]].local);
		localToWorld(k, heirarchy[rootID][i]);
	}
}

void ColladaParser::worldBindMatrix(int parentID)
{

	for (int i = 0; i < heirarchy[parentID].size(); i++)
	{
		bindMatrix[heirarchy[parentID][i]] = bindMatrix[parentID] * bindMatrix[heirarchy[parentID][i]];
		worldBindMatrix(heirarchy[parentID][i]);
	}
}

// ....................OUTPUT................................
void ColladaParser::saveAnim()
{
	std::string outName = (this->fileName) + ".dyn";
	outFile = new std::ofstream(outName);

	////////////////////////output skeleton/////////////////////////////////
	
	//	geometry
	for (int i = 0; i < vertexData.size(); i++)
	{
		(*outFile) << vertexData[i] << " ";
	}
	(*outFile) << std::endl;

	// indices
	for (int i = 0; i < indices.size(); i++)
	{
		(*outFile) << indices[i] << " ";
	}
	(*outFile) << std::endl;

	// jointData
	(*outFile) << noOfJoints << std::endl;
	for (std::map<std::string, int>::iterator it = jointMap.begin(); it != jointMap.end(); ++it)
	{
		(*outFile) << (it->first) << " " << (it->second) << std::endl;
	}

	//////////////////////output animations//////////////////////////////
	(*outFile) << animations->noOfAnims << std::endl;
	
	for (int i = 0; i < ( animations->noOfAnims ); i++)
	{
		int nKeys = animations->get(i)->noOfKeys;
		(*outFile) << (animations->get(i)->name) << " ";
		(*outFile) << nKeys << std::endl;
		for (int j = 0; j < nKeys; j++)
		{
			for (int k = 0; k < noOfJoints; k++)
			{
				JointData* jd = &(animations->get(i)->keys[j].joints[k]);
				(*outFile) << ( jd->rotation ) << " " << ( jd->translation )<<" ";
			}
			(*outFile) << std::endl;
		}
	}
	

	outFile->close();
}