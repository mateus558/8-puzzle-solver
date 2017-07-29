#include <iostream>
#include <algorithm>
#include "MinHeap.h"

using namespace std;

MinHeap::MinHeap(){
	this->size = 0;
}

int MinHeap::Size(){
	return size;
}

bool MinHeap::isEmpty(){
	return (size == 0);
}

int MinHeap::parent(int i){
	return (i-1)/2;
}

int MinHeap::left(int i){
	return (2*i + 1);
}

int MinHeap::right(int i){
	return (2*i + 2);
}

bool MinHeap::inHeap(Board b){
	for(Board node:nodes){
		if(node == b){
			return true;
		}
	}
	
	return false;
}

void MinHeap::insertKey(Board k){
	int i;
	
	size++;
	i = size - 1;
	nodes.resize(size);
	nodes[i] = k;
	
	while(i != 0 && nodes[parent(i)].getCost() > nodes[i].getCost()){
		iter_swap(this->nodes.begin()+parent(i), this->nodes.begin()+i);
		i = parent(i);	
	}
}

Board MinHeap::extractMin(){
	if(size <= 0){
		return Board();
	}
	if(size == 1){
		size--;
		return nodes[0];
	}
	
	Board root = nodes[0];
	nodes[0] = nodes[size-1];
	size--;
	minHeapify(0);
	
	return root;
}

void MinHeap::minHeapify(int i){
	int l = left(i), r = right(i), menor;
	Board temp;
	
	if(l < size && nodes[l].getCost() < nodes[i].getCost()){
		menor = l;
	}else{
		menor = i;
	}
	
	if(r < size && nodes[r].getCost() < nodes[menor].getCost()){
		menor = r;
	}
	
	if(menor != i){
		temp = nodes[i];
		nodes[i] = nodes[menor];
		nodes[menor] = temp;
		
		minHeapify(menor);
	}
}
