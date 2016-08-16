#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "npuzzle.h"

using namespace std;

bool sair = false;
bool inva = false;
int size, depth;
int** puzzle;
node* solution;

void clear(void);
void waitUserAction(void);
void showMenu(void);
void selectOption(int);

int main(int argc, char** argv){
	string opt;
	int o;
	
	while(true){
		if(sair){ 
			return 0;
		}
		clear();
		showMenu();
		cin >> opt;		
		istringstream iss(opt);
		iss >> o; 
		clear();
		selectOption(o);
	}

	return 0;		
}

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

void showMenu(){
	cout << "         *----------------------------------------------------------* " << endl;
	cout << "         *                 8-Puzzle Solver                     * " << endl;
	cout << "         *----------------------------------------------------------* " << endl;
	cout << endl;
	cout << "Select an option:\n" << endl;
	cout << "1 - Load Puzzle (typing)" << endl;
	cout << "2 - Load Puzzle (File)" << endl;
	cout << "3 - Solve Puzzle" << endl;
	cout << "4 - Show steps to solve" << endl; 
	cout << "5 - Show Puzzle" << endl;	
	cout << "6 - Exit" << endl;	
	if(inva){
		cout << endl;
		cout << "Invalid option." << endl;
		inva = false;
		cout << endl;
	}
	cout << " > ";	
}

void selectOption(int o){
	int i, j;
	string file;
	
	switch(o){
		case 1:{
				cout << "Enter the size: ";
				cin >> size;
				cout << "Depth limit: ";
				cin >> depth;
				
				puzzle = new int*[size];
				
				for(i = 0; i < size; i++){
					puzzle[i] = new int[size];
				}
				
				for(i = 0; i < size; i++){
					for(j = 0; j < size; j++){
						cout << "puzzle[" << i << "][" << j << "] = ";
						cin >> puzzle[i][j];
					}	
					cout << endl;
				}
				cout << "Puzzle Loaded..." << endl;
				waitUserAction();
				break;
			}
		case 2:{
				cout << "Enter the size: ";
				cin >> size;
				cout << "Depth limit: ";
				cin >> depth;
				cout << "File name: ";
				cin >> file;
				
				puzzle = new int*[size];
				
				for(i = 0; i < size; i++){
					puzzle[i] = new int[size];
				}
				for(i = 0; i < size; i++){
					for(j = 0; j < size; j++){
						puzzle[i][j] = 0;
					}
				}
				
				ifstream input(file.c_str(), ios::in);
				
				if(!input){
					cout << "\nFile could not be opened!" << endl;
					return;
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
				NPuzzle problem(puzzle, size);
				node* sol = problem.solve(depth);
				solution = sol;
				cout << "\nSolution:\n";
				if(sol != NULL){
					for(int i = 0; i < size; i++){
						for(int j = 0; j < size; j++){
							cout << sol->cand[i][j] << "\t";
						}
						cout << endl;
					}
					cout << "Moves: " << sol->depth << endl;
				}else{ cout << "This puzzle could not be solved, sorry..." << endl; }
				waitUserAction();
				break;
			}
		case 4:{
				unsigned int micsecs = 500000;
				vector<node*> came_from;
				came_from.push_back(solution);
				node* r_itr = solution->came_from;
				
				while(r_itr != NULL){
					came_from.push_back(r_itr);
					r_itr = r_itr->came_from;
				}
				
				vector<int> commands;
				
				for(i = came_from.size() - 1; i >= 0; i--){
					node* c = came_from[i];
					if(came_from[i]){
						for(int k = 0; k < c->size; k++){
							for(j = 0; j < c->size; j++){
								if(c->cand[k][j] == 0)
									cout <<"\033[42;30m" <<c->cand[k][j] << "\033[0m\t";
								else
									cout << c->cand[k][j] << "\t";
							}
							cout << endl;
						}
						cout << "-----------------" << endl;
						cout << endl;
					}
					commands.push_back(c->command);
					usleep(micsecs);
					if(i != 0)
						clear();
				}
				cout << endl;
				for(i = 0; i < commands.size(); i++){
					int c = commands[i];
					if(c == 0){
						cout << "RIGHT";
					}else if(c == 1){
						cout << "LEFT";
					}else if(c == 2){
						cout << "UP";
					}else if(c == 3){
						cout << "DOWN";
					}
					if(i != commands.size()-1){
						cout << ", ";
					}
				}
				cout << endl;
				
				waitUserAction();
				break;
			}
		case 5:{
				for(i = 0; i < size; i++){
					for(j = 0; j < size; j++){
						cout << puzzle[i][j] << "\t";
					}
					cout << endl;
				}
				cout << endl;
				waitUserAction();
				break;
			}
		case 6:{
			
				sair = true;
				break;
			}
		default:
			inva = true;
			break;
	}		
}



