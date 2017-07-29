#ifndef SOLVER_H_INCLUDED
#define SOLVER_H_INCLUDED

#include <vector>
#include "Board.h"
#include "MinHeap.h"

class Solver{
private:
	int N, moveSoFar;
	Board initial, goal, current;
	MinHeap open;
	std::vector<Board> closed, sol;
	
public:
	Solver(Board initial, bool verbose = false);
	bool inClosedSet(Board state);
	bool isSolvable();
	int moves();
	std::vector<Board> solution();	
};

#endif
