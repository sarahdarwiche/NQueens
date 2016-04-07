#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include <string>
#include "Move.h"


using namespace std;

class Search {

  public:
  
  int queen_count;
  int k;

  vector<int> initial_state;   
  vector<int> state; 
  vector<int> best_state;
  int least_violations;
  int violations;
  int restart_count;
  int moves_count;
   
  public:
  
  Search();

  void read_from_stdin();
  void generate_random_problem(int queen_count);
  void print_to_stdout(vector<int> &state); 
  void print_grid(vector<int> &state);
  int count_violations(vector<int> &state);
  int update_violations(vector<int> &state, int violations, int queen, int new_row );
  Move* get_best_neighbor();
  int hill_climb(clock_t start);
  void hill_climb_with_restart(clock_t start);
  void copy_state(vector<int> &original, vector<int> &copy, int size);
  int same_state(vector<int> &first, vector<int> &second, int size);
  void random_state(vector<int> &state);
  int satisfies_upper_bound(vector<int> &state, int size);
   
};

#endif