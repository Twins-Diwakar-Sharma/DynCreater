#ifndef MESH_INDEX_PED
#define MESH_INDEX_PED

#include <map>

class MIndexTree
{
private:
	std::map<int, std::map<int, int>>* indexMap;
	int length;
	int currentIndex = -1;

public:
	MIndexTree(int);
	~MIndexTree();
	
	int fetchIndex(int, int, int, bool*);
};

#endif