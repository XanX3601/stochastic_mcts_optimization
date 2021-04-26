#ifndef EXPERIMENTS_GRID_H
#define EXPERIMENTS_GRID_H

#include <twm/Board.h>
#include <twm/Problem.h>

namespace experiments {
namespace twm {
const double NEIGHBORS_IGNITION_PROBABILITY_GRID_1 = .06;

::twm::Problem* generate_problem_1(int grid_size, int team_count);
::twm::Board* generate_grid_1(::twm::Problem* problem);
}  // namespace twm
}  // namespace experiments

#endif /* ifndef EXPERIMENTS_GRID_H */
