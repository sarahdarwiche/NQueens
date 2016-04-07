#include "Search.h"
#include "Move.h"

#include <iostream>
#include <string>
#include <ctime>
#include <stdlib.h>     


int main (int argc, char* argv[]) {

    //srand (time(NULL));
    
    //start time
    std::clock_t start;
    double duration;
    start = std::clock();
    
    //create search object
    Search N_queen;
  
    int size = atoi(argv[1]);
    //reading search details from standard input  
    //N_queen.read_from_stdin();
    N_queen.generate_random_problem(size);

    /*   
    //count violations
    int current_violations = N_queen.count_violations(N_queen.initial_state);
    cout << "Number of violations (initial state): " << current_violations << endl;
    N_queen.print_grid(initial_state);
    for(int i=0; i< N_queen.queen_count; i++) cout << "***"; 
    cout << endl;*/
          
    //solve
    N_queen.hill_climb_with_restart(start);
    
    //end time
    duration = ((double)(std::clock()-start))/CLOCKS_PER_SEC;

    //printing my info
    cout << "restart count: " << N_queen.restart_count << endl;
    cout << "moves   count: " << N_queen.moves_count << endl;
    
    //printing solution of search
    cout << N_queen.least_violations << " conflicts"  <<  endl; 
    N_queen.print_to_stdout(N_queen.best_state);
    std::cout << duration << " seconds" << endl;

    N_queen.print_grid(N_queen.best_state);
/*
    for(int i=0; i< N_queen.queen_count; i++) cout << "***"; 
    cout << endl;
	cout << "Number of violations (final state): "  << N_queen.count_violations(N_queen.best_state) << endl;    
    N_queen.print_grid(N_queen.best_state);*/
  
  return 0;
 
}