#include "MIndexTree.h"

MIndexTree::MIndexTree(int noOfVertices)
{
	indexMap = new std::map<int, std::map<int, int>>[noOfVertices];
	length = noOfVertices;
}

MIndexTree::~MIndexTree()
{
	delete[] indexMap;
}

int MIndexTree::fetchIndex(int v, int t, int n, bool * alreadyThere)
{
	if (indexMap[v].count(t) == 1 && indexMap[v][t].count(n) == 1)
	{
		*(alreadyThere) = true;
		return indexMap[v][t][n];
	}
	else   // create new
	{
		*(alreadyThere) = false;
		currentIndex++;
		if (indexMap[v].count(t) < 1)	// the entire texture tree was not there
		{
			std::map<int, int> normalToIndexMap;
			normalToIndexMap.insert(std::pair<int,int>(n,currentIndex));	
			indexMap[v].insert(  std::pair< int, std::map<int, int> >(t, normalToIndexMap) );
		}
		else if (indexMap[v].count(t) == 1 && indexMap[v][t].count(n) < 1) // texture tree is there but not this particular normal in that tree
		{
			indexMap[v][t].insert(std::pair<int,int>(n,currentIndex));
		}

		return currentIndex;
	}
}
