#include <iostream>
#include "Solver.h"

using namespace std;

Solver::Solver(Board initial, bool verbose){
	int i, csize, added;
	vector<Board> neighborStates;
	
	moveSoFar = i = 0;
	
	initial.setAnt(Board());
	initial.setMoves(moveSoFar);
	initial.computeCost();
	open.insertKey(initial);
	
	while(!open.isEmpty()){
		current = open.extractMin();
		closed.push_back(current);
		
		if(i%100 == 0 && verbose){
			cout << "Nodes in memory: " << open.Size() + closed.size() << endl;
			cout << "Current node cost: " << current.getCost() << endl;
			cout << "Depth: " << current.getMoves() << endl; 
			cout << endl;	
		}	
		
		if(current.isGoal()){ 
			goal = current;
			Board *curr = &goal, null = Board();
			while(!(*curr == null)){
				sol.insert(sol.begin(), *curr);
				curr = curr->getAnt();
			}
			cout << "Nodes in memory: " << open.Size() + closed.size() << endl;
			cout << "Current node cost: " << current.getCost() << endl;
			cout << "Depth: " << current.getMoves() << endl; 
			cout << "\nGoal reached!" << endl;
			return;
		}

		neighborStates = current.neighbors();

		for(Board state : neighborStates){
			if(inClosedSet(state) || state == *state.getAnt()){
				continue;
			}
			
			if(!open.inHeap(state)){
				state.computeCost();
				state.setAnt(current);
				state.setMoves(current.getMoves() + 1);
				state.changeCost(state.getMoves());
				open.insertKey(state);
			}
		} 
	
		i++;
	}
	cout << "No more open states..." << endl;
}

bool Solver::inClosedSet(Board state){
	for(Board S : closed){
		if(S == state)
			return true;
	}
	return false;
}

bool Solver::isSolvable(){

}

int Solver::moves(){
	return moveSoFar;
}

std::vector<Board> Solver::solution(){
	
	return sol;
}
