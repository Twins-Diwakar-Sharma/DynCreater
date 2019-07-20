#ifndef H_COLLADA_PARSER_H
#define H_COLLADA_PARSER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "MIndexTree.h"
#include "Mathematics.h"
#include "Animations.h"

class ColladaParser
{
public:
	ColladaParser(std::string);
private:

	int error = 0;

	std::string fileName;
	std::ifstream *inFile;
	std::ofstream *outFile;

	std::vector<float> vertexData;	// the grandDaddy
	std::vector<int> indices;
	std::map<std::string, int> jointMap;	// jointName to jointNo map
	std::vector<std::vector<int>> heirarchy;	// skeleton heirarchy
	Animations *animations;	// animations


	int noOfJoints;
	Matrix4f bindShapeMatrix;
	Matrix4f *invBindMatrix;

	Matrix4f *bindMatrix;
	Matrix4f armatureMatrix;

	int rootID;

	void initialize();
	
	void saveAnim();

	void loadAnim(std::string& type, bool defaultSetup);
		void library_geometries(std::vector<float> data[], std::vector<int>& p);
		void library_controllers(std::vector<float> data[], std::vector<int>& vcount, std::vector<int>& v);
		void library_visual_scenes();
		void library_animations(std::string type);
		// ...................calculations.........................
		void createVertexData(std::vector<float> data[], std::vector<int>& p, std::vector<int>& vcount, std::vector<int>& v);
		void recursiveHeirarchy(int parentID, std::string&);
		void generateWorldMatrices(std::string& type);
		void localToWorld(KeyFrame&,int);
		void worldBindMatrix(int parentID);
		
};


#endif
