#include "ColladaParser.h"

int main()
{
	std::string fileName("");
	std::cout << "Enter filename without suffix " << std::endl;
	std::cin >> fileName;

	int noOfAnims = 0;
	std::cout << "\n\nEnter no of animations :  ";
	std::cin >> noOfAnims;

	std::vector<std::string> types;
	std::cout << "\n\nEnter each animation's name " << std::endl;
	for (int i = 0; i < noOfAnims; i++)
	{
		std::cout << "Animation name " << i << " : ";
		std::string type;
		std::cin >> type;
		types.push_back(type);
	}

	ColladaParser colladaParser(fileName,types);
}