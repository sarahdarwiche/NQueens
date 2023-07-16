#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>

using namespace std;
#include "Search.h"



/*****************************************************************************************
 * constructors
 ****************************************************************************************/
 
//constructor
Search::Search() {
}

/*****************************************************************************************
 * input / output
 ****************************************************************************************/

//reading game from standard input
void Search::read_from_stdin() {

  cin >> queen_count; 
  cin >> k; 
  initial_state.resize(queen_count);

  int i = queen_count;
  while(i--) {
    int row, column;
    cin >> row; //rows start from 1
    cin >> column; //columns start from 1
    //our current queen has id = column-1 and exist in row-1
    initial_state[column - 1] = row - 1;
  }
  
}

//reading game from standard input
void Search::generate_random_problem(int size) {

  queen_count = size; 
  initial_state.resize(queen_count);
  k = (rand() % (queen_count*queen_count - 1)); 
  cout << "k "  << k << endl;
  
  random_state(initial_state);
}


void Search::print_to_stdout(vector<int> &state) {

  for(int queen = 0; queen < queen_count; queen++) {
    int column = queen + 1;
    int row    = state[queen] + 1;
    cout << row  << " " <<  column  << endl; 
  }
   
}

void Search::print_grid(vector<int> &state) {

  for(int r = 0; r < queen_count; r++) {
	for (int c = 0; c < queen_count; c++) {
	  if(state[c] == r) cout << " Q ";
	  else cout << " - ";
  }	
cout << endl;

}

}

int Search::count_violations(vector<int> &state) { //make linear

  int violations= 0;
  
  for(int current = 0; current < queen_count - 1; current++) {
  	for(int next = current + 1; next < queen_count; next++) {
  		if(state[current] == state[next]) violations++;
  		if(abs(current-next) == abs(state[current] - state[next])) violations++;
  	}
  }
  
  return violations;
}

int Search::update_violations(vector<int> &state, int violations, int queen, int new_row ) {
 
  int new_violations = 0;
  int old_violations = 0;
 
  int old_row = state[queen];
  for(int q = 0; q < queen_count; q++) {
    if(q != queen) {
      if(state[q] == new_row) new_violations++;
  	  if(abs(queen-q) == abs(state[q] - new_row)) new_violations++;
  	  
  	  if(state[q] == old_row) old_violations++;
  	  if(abs(queen-q) == abs(state[q] - old_row)) old_violations++;
    }
  }
   
   violations = violations + new_violations - old_violations;
   return violations;
  
}

Move* Search::get_best_neighbor() {
  Move* best_move = new Move;
  
  int min_violations = queen_count * (queen_count - 1);
  
  for(int queen = 0; queen < queen_count; queen++) {
  	int original_row = state[queen]; //save state
  	
  	for(int row = 0; row < queen_count; row++) {
  		if(row != original_row) {
  			int new_violations = update_violations(state,violations, queen, row );
  			state[queen] = row; 
  			//if( new_violations != count_violations(state)) cout << "ERROR!!!!!!!!" << endl;
  			//cout << "Row: " << row << " Column: " << queen << " Number of violations: " << new_violations << endl;
  			if(new_violations <= min_violations) {
  				best_move->queen = queen;
  				best_move->violations = new_violations;
  				best_move->row = row;
  				min_violations = new_violations;
  			 }
  		 }
  	}
  	
  	state[queen] = original_row;	//reset to current state
  }
    return best_move;
}  


int Search::hill_climb(clock_t start) {
	int time_limit = 30*60-5;
	
	int found;
	vector<int> prev_state;
	vector<int> next_state;
	prev_state.resize(queen_count);
	next_state.resize(queen_count);
	copy_state(state,prev_state,queen_count);
	copy_state(state,next_state,queen_count);
	do {
		Move* best_neighbor = get_best_neighbor();
		found = best_neighbor->violations <= violations;
		if(found) {
		    next_state[best_neighbor->queen] = best_neighbor->row;
		    if(same_state(prev_state,next_state,queen_count)) return 1;
		    ++moves_count;
		    copy_state(state,prev_state,queen_count);
			state[best_neighbor->queen] = best_neighbor->row;
			violations = best_neighbor->violations;
			if(violations < least_violations) {
			  least_violations = violations;
			  copy_state(state,best_state,queen_count);
			}
			free(best_neighbor);
		}
	
	double seconds_so_far = ((double)(clock()-start))/CLOCKS_PER_SEC;
    if(seconds_so_far > time_limit) return 0;
	
	} while(found);
	
	return 1;
}

void Search::hill_climb_with_restart(clock_t start) {
 
    
  restart_count = 0;
  moves_count   = 0;
  
  //set state to be initial state
  state.resize(queen_count);
  copy_state(initial_state,state,queen_count);
  
  //set best state to be initial state
  best_state.resize(queen_count);
  copy_state(initial_state,best_state,queen_count);
  
  least_violations = count_violations(initial_state); //must be counted
  violations = least_violations;
  
  if( violations != count_violations(state)) {
  			cout << " error 1 " << endl; 
  			exit (EXIT_FAILURE);
  			}
  hill_climb(start); //based on given initial state
   
  while(violations != 0 || !satisfies_upper_bound(state,queen_count)) { 
    ++restart_count;
    random_state(state);
    violations = count_violations(state);
    if( violations != count_violations(state)) {
  			cout << " error 2" << endl; 
  			exit (EXIT_FAILURE);
  			}
    if(!(hill_climb(start))) return;
  }
  
}

void Search::copy_state(vector<int> &original, vector<int> &copy,int size) {
  for(int i = 0; i < size; i++) copy[i] = original[i];
}

int Search::same_state(vector<int> &first, vector<int> &second, int size){
  for(int i = 0; i < size; i++) {
    if(first[i] != second[i]) return 0;
  }
  return 1;
}

void Search::random_state(vector<int> &state) {
  for(int i = 0; i < queen_count; i++) state[i] = (rand() % queen_count);
}

int Search::satisfies_upper_bound(vector<int> &state, int size) {
  //check whether state and initial_state satisfy upper bound K
  int sum = 0;
  for( int i = 0; i < size; i++) {
  	sum = sum + abs(state[i] - initial_state[i]); 
  }
  return k >= sum;
}

/*****************************************************************************************
 * end
 ****************************************************************************************/
  

