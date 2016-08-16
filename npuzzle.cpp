#include <iostream>
#include <cmath>
#include <stack>
#include <algorithm>
#include "npuzzle.h"

using namespace std;

node::node(int size){
	moves.reserve(4);
	this->size = size;
	for(int i = 0; i < 4; i++){
		moves[i] = NULL;
	}
}

NPuzzle::NPuzzle(int** instance, int size){
	root = new node(size);
	
	root->came_from = NULL;
	root->cand = instance;
	moves = 0;
	this->size = size;
	visited = new bool[size];
	
	for(int i = 0; i < size; i++, visited[i] = false);
		
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(instance[i][j] == 0){
				root->pos.x = i;
				root->pos.y = j;
			}
		}
	}
	root->depth = 0;
	root->num = 0;
	root->dis = manhattan_distance(root->cand);
}

int NPuzzle::manhattan_distance(int** cand){
	int i, j, sum;
	Point orig;
	
	for(sum = 0, i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			orig.x = cand[i][j]%size;
			orig.y = (cand[i][j] - orig.x)/size;
			sum += fabs(i - orig.x) + fabs(j - orig.y); 	
		}
	}
	
	return sum;
}

int** NPuzzle::move(Point p1, Point p2, int** cand){
	int** cand1 = new int*[size];
	for(int i = 0; i < size; i++){
		cand1[i] = new int[size];
	}
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			cand1[i][j] = cand[i][j];
		}
	}
	
	int temp = cand1[p2.x][p2.y];
	
	cand1[p2.x][p2.y] = cand[p1.x][p1.y];
	cand1[p1.x][p1.y] = temp;
	
	return cand1;
}

bool NPuzzle::is_solution(node *root){
	
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(root->cand[i][j] != (i*size + j)){ return false; }
		}
	}
	
	return true;
}

vector<node*> NPuzzle::set_neigh(node* root, Point ant){
	int x, y, temp;
	x = root->pos.x;
	y = root->pos.y;
	node* moves[4];
	vector<node*> pos_moves;
	Point orig;
	if(x < size - 1){
		moves[0] = new node(size);
		moves[0]->command = 0;
		moves[0]->pos.x = root->pos.x+1;
		moves[0]->pos.y = root->pos.y;
		moves[0]->cand = move(root->pos, moves[0]->pos, root->cand);
		moves[0]->num = root->cand[x+1][y];
		moves[0]->dis = manhattan_distance(moves[0]->cand);
		moves[0]->depth = root->depth + 1;
		if(moves[0]->pos.x != ant.x && moves[0]->pos.y != ant.y)
			pos_moves.push_back(moves[0]);
	}
	if(x > 0){
		moves[1] = new node(size);
		moves[1]->command = 1;
		moves[1]->pos.x = root->pos.x-1;
		moves[1]->pos.y = root->pos.y;
		moves[1]->cand = move(root->pos, moves[1]->pos, root->cand);
		moves[1]->num = root->cand[x-1][y];
		moves[1]->dis = manhattan_distance(moves[1]->cand);
		moves[1]->depth = root->depth + 1;
		if(moves[1]->pos.x != ant.x || moves[1]->pos.y != ant.y)
			pos_moves.push_back(moves[1]);
	}
	if(y > 0){ 
		moves[2] = new node(size);
		moves[2]->command = 2;
		moves[2]->pos.x = root->pos.x;
		moves[2]->pos.y = root->pos.y-1;
		moves[2]->cand = move(root->pos, moves[2]->pos, root->cand);
		moves[2]->num = root->cand[x][y-1];
		moves[2]->dis = manhattan_distance(moves[2]->cand);
		moves[2]->depth = root->depth + 1;
		if(moves[2]->pos.x != ant.x || moves[2]->pos.y != ant.y)
			pos_moves.push_back(moves[2]);
	}
	if(y < size-1){
		moves[3] = new node(size);
		moves[3]->command = 3;
		moves[3]->pos.x = root->pos.x;
		moves[3]->pos.y = root->pos.y+1;
		moves[3]->cand = move(root->pos, moves[3]->pos, root->cand);
		moves[3]->num = root->cand[x][y+1];
		moves[3]->dis = manhattan_distance(moves[3]->cand);
		moves[3]->depth = root->depth + 1;  
		if(moves[3]->pos.x != ant.x || moves[3]->pos.y != ant.y)
			pos_moves.push_back(moves[3]);
	}
	
	return pos_moves;
}

bool comparator(node* a, node* b){
	return a->dis < b->dis;
}

void NPuzzle::search_solution(node *root, int depth_lim, vector<node*> &cand){
	node *top, *ant;
	bool flag = false;
	int d = 0, num = 0;
	stack<node*> minHeap;
	vector<node*>::iterator itr;
	
	if(depth_lim == 0){
		cand.push_back(root);
		return;
	}
	
	minHeap.push(root);

	while(!minHeap.empty()){
		top = minHeap.top();
		minHeap.pop();		
		if(top->depth == depth_lim+1){ cout << num << endl; break; }			
		vector<node*> neigh = set_neigh(top, (!ant)?Point(-1,-1):ant->pos);
		top->moves = neigh;
		ant = top;
		sort(neigh.begin(), neigh.end(), comparator);
		for(int i = 0; i < neigh.size(); i++){
			neigh[i]->came_from = top;
			node* u = neigh[i];;
			if(u != NULL && u->depth == depth_lim){
				cand.push_back(u);
			}
			if(u != NULL && u->depth < depth_lim && u->num != top->num){
				minHeap.push(u);
			}
		}
	}	
	//cout << num << endl;
}

void clear_children(node* root){
	if(root == NULL) return;
	for(int i = 0; i < root->moves.size(); i++){
		delete root->moves[i];
	}
}

node* NPuzzle::solve(int lim_depth){
	int depth = 0;
	bool flag;
	node* sol = NULL;
	vector<node*> temp;
	
	for(depth = 0; depth <= lim_depth; depth++){
		cout << "Depth: " << depth << endl;
		clear_children(root);
		root->depth = 0;
		search_solution(root, depth, temp);
		for(node* cand : temp){ 
			if(is_solution(cand)){ sol = cand; return sol;} 		
		}
		temp.clear();
	}
	
	return NULL;
}

node::~node(){
	for(int i = 0; i < size; i++){
		delete [] cand[i];
	}
	delete cand;
	moves.clear();
}

