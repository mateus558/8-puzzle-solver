#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ctime>
#include "unistd.h"
#include "Board.h"
#include "MinHeap.h"
#include "Solver.h"

using namespace std;

bool inva = false;
int size;

void clear(void){
#ifdef __unix__
	system("clear");
#elif _WIN32
	system("CLS");
#endif
}

void waitUserAction(void){
	cout << "\nPress ENTER to continue..." << endl;
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	cin.get();
}

int posicaoValida(int **M, int m, int n, int r){
	int i, j, existe = 0;
	
	for(i = 0; i < m; i++){
		for(j = 0; j < n; j++){			
			if(M[i][j] == r){
				return 0;						
			}
		}
	}
	
	return 1;
}

int** randGame(int m, int n, int *zeroi, int *zeroj, unsigned int *seed){
	int i, j, r, k, existe;
	int **M;
	
	if(*seed == 0){
		*seed = time(NULL);
	}
	
	srand(*seed);
	
	//Aloca uma matriz dinamicamente na memória com todas posições iguais a -1
	M = (int **)malloc(m * sizeof(int*));
	
	for(i = 0; i < m; i++){
		M[i] = (int *)malloc(n * sizeof(int));
		for(j = 0; j < n; j++){
			M[i][j] = -1;
		}
	}
	
	for(i = 0; i < m; i++){
		for(j = 0; j < n; j++){
			//Enquanto a posição não for válida pega um novo valor aleatório
			do{
				r = rand() % 9;
			}while(!posicaoValida(M, m, n, r));
			
			if(r == 0){
				*zeroi = i;
				*zeroj = j;
			}
			
			M[i][j] = r;	
		}
	}
	
	return M;
}

void showMenu(){
	cout << "         *----------------------------------------------------------* " << endl;
	cout << "         *                 8-Puzzle Solver                     * " << endl;
	cout << "         *----------------------------------------------------------* " << endl;
	cout << endl;
	cout << "Select an option:\n" << endl;
	cout << "1 - Load Puzzle (typing)" << endl;
	cout << "2 - Load Puzzle (File)" << endl;
	cout << "3 - Random game " << endl;
	cout << "4 - Solve Puzzle" << endl;
	cout << "5 - Show steps to solve" << endl; 
	cout << "6 - Show Puzzle" << endl;	
	cout << "7 - Exit" << endl;	
	if(inva){
		cout << endl;
		cout << "Invalid option." << endl;
		inva = false;
		cout << endl;
	}
	cout << " > ";	
}


int main(int argc, char** argv){
	int i, j, op, zi, zj;
	unsigned int micsecs = 500000, seed;
	int **puzzle = NULL, zeroi, zeroj;
	string file;
	Board initial;
	vector<Board> sol;
	
	while(true){
		clear();
		showMenu();
		cin >> op;
		clear();
		switch(op){
			case 1:{
					MinHeap queue;
					cout << "Enter the size: ";
					cin >> size;
					
					if(puzzle == NULL){
						puzzle = new int*[size];
						for(i = 0; i < size; i++){
							puzzle[i] = new int[size];
							for(j = 0; j < size; j++){
								puzzle[i][j] = 0;
							}
						}	
					}
					
					for(i = 0; i < size; i++){
						for(j = 0; j < size; j++){
							cout << "puzzle[" << i << "][" << j << "] = ";
							cin >> puzzle[i][j];
							if(puzzle[i][j] == 0){
								zi = i;
								zj = j;
							}
						}	
						cout << endl;
					}
					initial = Board(puzzle, size, 1);	
					initial.setZeroPos(zi, zj);
					
					cout << "Puzzle Loaded..." << endl;
					waitUserAction();
					break;
				}
			case 2:{
					cout << "Enter the size: ";
					cin >> size;
					cout << "File name: ";
					cin >> file;
				
				
					ifstream input(file.c_str(), ios::in);
				
					if(!input){
						cout << "\nFile could not be opened!" << endl;
						return 0;
					}

					for(i = 0; i < size; i++){
						for(j = 0; j < size; j++){
							input >> puzzle[i][j];
						}	
					}

					cout << "Puzzle Loaded..." << endl;
					waitUserAction();
					break;
				}
			case 3:{
					clear();
					int i, j;
					cout << "Enter the size: ";
					cin >> size;
					cout << "Enter the seed: ";
					cin >> seed;
					
					puzzle = randGame(size, size, &zeroi, &zeroj, &seed);
					initial = Board(puzzle, size, 1);	
					initial.setZeroPos(zeroi, zeroj);
					
					cout << "\nSeed: " << seed << "\n" << endl;
					
					for(i = 0; i < size; i++){
						for(j = 0; j < size; j++){
							cout << puzzle[i][j] << " ";
						}
						cout << endl;
					}
					
					cout << "\nPuzzle Generated..." << endl;
					waitUserAction();
					
					break;
				}
			case 4:{
					unsigned int micsecs = 500000;
					bool v;
					
					cout << "Verbose? ";
					cin >> v; 
					
					Solver npuzzle(initial, v);
					
					sol = npuzzle.solution();
					
					cout << sol.size()-1 << " moves were made." << endl;
					
					waitUserAction();
					break;
				}
			case 5:{
					for(Board state : sol){
						clear();
						cout << state.getMoves() << endl;
						cout << "State cost: " << state.getCost() << endl; 
						cout << state << endl;
						usleep(micsecs);
					}
					cout << sol.size()-1 << " moves were made." << endl;
					
					waitUserAction();
					break;
				}
			case 6:{
					cout << initial << endl;
					waitUserAction();
					break;
				}
			case 7:			
					return 0;
					break;
			default:
				//inva = true;
				break;
		}
	}
	
	return 0;		
}






