#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
#include <vector>
#include <string>
#include <ostream>

typedef std::vector<std::vector<int> > iMatrix;

class Board{
private:
	int N, zeroi, zeroj;
	int cost, cost_type, moves;
	Board *ant;
	iMatrix tiles;
public:
	Board() : cost_type(0), N(0), zeroi(-1), zeroj(-1) {};
	Board(int** tiles, int N, int cost_type = 0);
	void setZeroPos(int zeroi, int zeroj);
	void computeCost();
	void copy(const Board &y);
	void setAnt(Board ant);
	Board* getAnt(){return ant;};
	void setMoves(int moves){this->moves = moves;}
	int getMoves(){return moves;}
	void changeCost(int delta);
	int getCost();
	int hamming();
	int manhattan();
	void execAction(int op, Board &new_state);
	std::vector<Board> neighbors();
	bool isEmpty();
	bool isGoal();
	bool operator==(const Board&);
	void operator=(const Board&);
	friend std::ostream &operator<<( std::ostream &output, const Board &board );
	~Board();
};

#endif
