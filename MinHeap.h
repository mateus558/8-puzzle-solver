#ifndef MINHEAP_H_INCLUDED
#define MINHEAP_H_INCLUDED

#include <vector>
#include "Board.h"

class MinHeap{
private:
	int size;
	std::vector<Board> nodes;
	
	void swap(int i, int j);
public:
	MinHeap();
	int parent(int i);
	int left(int i);
	int right(int i);
	void minHeapify(int i);	
	void insertKey(Board key);
	bool inHeap(Board b);
	bool isEmpty();
	int Size();
	Board extractMin();
};

#endif
