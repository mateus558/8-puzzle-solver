#include <iostream>
#include <algorithm>

#include "Board.h"

using namespace std;

Board::Board(int** tiles, int N, int cost_type){
	int i, j;
	
	this->cost_type = cost_type;
	this->tiles = iMatrix(N, vector<int>(N, 0));
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			this->tiles[i][j] = tiles[i][j];
		}
	}
	this->N = N;
	zeroi = zeroj = -1;
}

void Board::setZeroPos(int zeroi, int zeroj){
	this->zeroi = zeroi;
	this->zeroj = zeroj;
}

int Board::hamming(){
	int i, j, pos, sum = 0;
	
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){

			if(i == N-1 && j == N-1){
				break;
			}
			pos = i*N + j + 1;
			if(tiles[i][j] != pos){
				sum += 1;
			}
		}
	}

	return sum;
}

int Board::manhattan(){
	int i, j, sum = 0, x, y, pos;
	
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			pos = i*N + j + 1;
			if(tiles[i][j] != 0 && tiles[i][j] != pos){
				x = (tiles[i][j]-1) % N;
				y = (tiles[i][j]-1) / N;

				sum += abs(i - y) + abs(j - x);
			}	
		}
	}
	
	return sum;
}

void Board::computeCost(){
	/*switch(cost_type){
		case 0:
			cost = manhattan();
			break;
		case 1:
			cost = hamming();
			break;
		default:
			cost = -1;
			break;
	}*/
	cost = manhattan();
}

void Board::changeCost(int delta){
	cost += delta;
}

int Board::getCost(){
	return cost;
}

void Board::execAction(int op, Board &new_state){
	int i, j, iz = zeroi, jz = zeroj, temp;
	int **tiles1 = new int*[N];
	bool flag = false;
	
	for(i = 0; i < N; i++){
		tiles1[i] = new int[N];
		for(j = 0; j < N; j++){
			tiles1[i][j] = tiles[i][j];
		}
	}
	
	temp = tiles1[zeroi][zeroj];
	switch(op){
		case 0:
			if(zeroi > 0){
				iz = zeroi-1;
				
				tiles1[zeroi][zeroj] = tiles1[iz][zeroj];
				tiles1[iz][zeroj] = temp;				
			}else{
				flag = true;
			}
			break;
		case 1:
			if(zeroj > 0){
				jz = zeroj-1;
				
				tiles1[zeroi][zeroj] = tiles1[zeroi][jz];
				tiles1[zeroi][jz] = temp;
			}else{
				flag = true;
			}
			break;
		case 2:
			if(zeroi < (N-1)){
				iz = zeroi+1;
				
				tiles1[zeroi][zeroj] = tiles1[iz][zeroj];
				tiles1[iz][zeroj] = temp;
			}else{
				flag = true;
			}
			break;
		case 3:
			if(zeroj < (N-1)){
				jz = zeroj+1;
				
				tiles1[zeroi][zeroj] = tiles1[zeroi][jz];
				tiles1[zeroi][jz] = temp;
			}else{
				flag = true;
			}
			break;
		default:
			break;
	}
	
	if(!flag){
		new_state = Board(tiles1, N, cost_type);
		new_state.setZeroPos(iz, jz);
	}else{
		new_state = Board();
	}
	
	for(i = 0; i < N; i++){
		delete [] tiles1[i];
	}
	delete [] tiles1;	
}

vector<Board> Board::neighbors(){
	Board B;
	vector<Board> Neigh;
	
	//UP
	execAction(0, B);
	if(!B.isEmpty()){
		Neigh.push_back(B);
	}
	
	//LEFT
	execAction(1, B);
	if(!B.isEmpty()){
		Neigh.push_back(B);
	}
	//DOWN
	execAction(2, B);
	if(!B.isEmpty()){
		Neigh.push_back(B);
	}
	//RIGHT
	execAction(3, B);
	if(!B.isEmpty()){
		Neigh.push_back(B);
	}
	
	return Neigh;
}

bool Board::isGoal(){
	int i, j;
	
	if(tiles[0][0] == 0) return false; 
	
	for(i = 0; i < N; i++){
		for(j = 0; j < (N-1); j++){
			if(i == (N-1) && (j+1) == N-1){ break; }
			if(tiles[i][j] > tiles[i][j+1]){
				return false;
			}
		}
		if((i != 2) && tiles[i][j] > tiles[i+1][0]){
			return false;
		}
	}
	
	return true;
}

bool Board::isEmpty(){
	return (N == 0);
}

void Board::setAnt(Board ant){
	this->ant = new Board();
	this->ant->copy(ant);
}

void Board::copy(const Board &y){
	int i, j;
	
	N = y.N;
	cost = y.cost;
	cost_type = y.cost_type;
	zeroi = y.zeroi;
	zeroj = y.zeroj;
	ant = y.ant;
	moves = y.moves;
	
	this->tiles = iMatrix(N, vector<int>(N, 0));
	
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			tiles[i][j] = y.tiles[i][j];
		}
	}	
}

bool Board::operator==(const Board& y){
	int i, j;
	if(N != y.N) return false;
	
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(tiles[i][j] != y.tiles[i][j]){
				return false;
			}
		}
	}	
	return true;
}

void Board::operator=(const Board& y){
	int i, j;
	
	N = y.N;
	cost = y.cost;
	cost_type = y.cost_type;
	zeroi = y.zeroi;
	zeroj = y.zeroj;
	ant = y.ant;
	moves = y.moves;
	
	this->tiles = iMatrix(N, vector<int>(N, 0));
	
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			tiles[i][j] = y.tiles[i][j];
		}
	}	
}

ostream &operator<<(ostream &output, const Board &board){
	int i, j;
	
	for(i = 0; i < board.N; i++){
		for(j = 0; j < board.N; j++){
			if(board.tiles[i][j] == 0){
				output << "\033[42;30m" << board.tiles[i][j] << "\033[0m "; 
			}else
				output << board.tiles[i][j] << " ";
		}
		output << endl;
	}	
	return output;
}

Board::~Board(){

}
