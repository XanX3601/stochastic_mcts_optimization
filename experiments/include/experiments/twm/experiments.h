#ifndef EXPERIMENTS_EXPERIMENTS_H
#define EXPERIMENTS_EXPERIMENTS_H

#include <experiments/twm/ExperimentResult.h>

namespace experiments {
namespace twm {
ExperimentResult experiment_uct_problem_1(int grid_size, int team_count,
                                          double c, int search_time);
}
}  // namespace experiments

#endif /* ifndef EXPERIMENTS_EXPERIMENTS8H */
