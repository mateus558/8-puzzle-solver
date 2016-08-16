#ifndef NPUZZLE_H_
#define NPUZZLE_H_
#include <vector>

using namespace std;

typedef vector<vector<int> > Field;

struct Point{
	int x, y;
	
	Point(){}
	Point(int x1, int y1){ x = x1; y= y1;} 
};

struct node{
	int** cand;
	int size;
	Point pos;
	int command;
	int num;
	int depth;
	int dis;
	node* came_from;
	vector<node*> moves;
	
	node(int size);
	~node();
};

class NPuzzle{
	node* root;
	bool *visited;
	int size;
	int moves;
	public:
	NPuzzle(){};
	NPuzzle(int** instance, int size);
	int manhattan_distance(int** cand);
	vector<node*> set_neigh(node* root, Point ant);
	bool is_solution(node *root);
	void search_solution(node *root, int lim_depth,vector<node*> &cand);
	node* solve(int lim_depth);
	int** move(Point p1, Point p2, int** cand);
};

#endif
