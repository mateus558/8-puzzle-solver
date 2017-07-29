all:
	g++ -g --std=c++11 Board.h Board.cpp Solver.h Solver.cpp MinHeap.h MinHeap.cpp main.cpp -o npuzzle
