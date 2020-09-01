#ifndef EXPERIMENTS_GRID_H
#define EXPERIMENTS_GRID_H 

#include <twm/Problem.h>
#include <twm/Board.h>

::twm::Problem generate_problem_1(int grid_size, int team_count);
::twm::Board generate_grid_1(twm::Problem* problem);

#endif /* ifndef EXPERIMENTS_GRID_H */
