#ifndef EXPERIMENTS_EXPERIMENTS_H
#define EXPERIMENTS_EXPERIMENTS_H

#include <experiments/twm/ExperimentResult.h>

namespace experiments {
namespace twm {
const std::string EXPERIMENT_TAG = "[experiment]";
const std::string UCT_TAG = "[uct]";
const std::string RANDOM_TAG = "[random]";
const std::string PROBLEM_1_TAG = "[problem 1]";
ExperimentResult experiment_uct_problem_1(int grid_size, int team_count,
                                          double c, int search_time);
ExperimentResult experiment_random_problem_1(int grid_size, int team_count);
}  // namespace twm
}  // namespace experiments

#endif /* ifndef EXPERIMENTS_EXPERIMENTS8H */
