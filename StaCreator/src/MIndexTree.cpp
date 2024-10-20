#include "MIndexTree.h"

MIndexTree::MIndexTree(int n)
{
	length = n;
	indexMap = new std::map<int, std::map<int, int>>[n];
}

MIndexTree::~MIndexTree()
{
	delete[] indexMap;
}

int MIndexTree::fetchIndex(int v, int t, int n, bool* alreadyExits)
{
	if (indexMap[v].count(t) == 1 && indexMap[v][t].count(n) == 1)
	{
		*(alreadyExits) = true;
		return indexMap[v][t][n];
	}
	else								      // create new
	{
		*(alreadyExits) = false;
		currentIndex++;
		if (indexMap[v].count(t) < 1)		// entire texture tree is missing
		{

			std::map<int, int> normalTextureMap;
			normalTextureMap.insert(std::pair<int, int>(n,currentIndex));

			indexMap[v].insert(std::pair<int, std::map<int, int>>(t, normalTextureMap));
		}
		else if (indexMap[v].count(t) == 1 && indexMap[v][t].count(n) < 1)
		{
			indexMap[v][t].insert(std::pair<int, int>(n, currentIndex));
		}

		return currentIndex;
	}
	
}
