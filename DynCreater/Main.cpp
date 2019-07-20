#include "ColladaParser.h"

int main()
{
	std::cout << "Enter filename without suffix " << std::endl;
	std::string fileName("");
	std::cin >> fileName;
	//std::string fileName("mushroom"); // no dae boom boom
	ColladaParser colladaParser(fileName);
}