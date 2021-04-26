#ifndef EXPERIMENTS_TWM_UCT
#define EXPERIMENTS_TWM_UCT

#include <string>

namespace experiments {
namespace twm {
namespace uct {
void solve_problem_1(int grid_size, int team_count, int solve_count, double exploration_parameter,
                     double search_time, std::string result_file_path);
}
}  // namespace twm
}  // namespace experiments

#endif /* ifndef EXPERIMENTS_TWM_UCT */
