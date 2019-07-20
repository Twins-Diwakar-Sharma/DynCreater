#ifndef MESH_LOADER_INDEX_TREE
#define MESH_LOADER_INDEX_TREE

#include <map>

class MIndexTree
{
private:
	std::map<int, std::map<int, int>> *indexMap;
	int currentIndex = -1, length;
public:
	MIndexTree(int);
	~MIndexTree();

	int fetchIndex(int v, int t, int n, bool* alreadyThere);
	
};
#endif