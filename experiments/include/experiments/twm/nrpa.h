#ifndef EXPERIMENTS_TWM_NRPA
#define EXPERIMENTS_TWM_NRPA

#include <string>

namespace experiments {
namespace twm {
namespace nrpa {
void solve_problem_1(int grid_size, int team_count, int solve_count, int root_level,
                     int iteration_count, double learning_step, std::string result_file_path);
}
}  // namespace twm
}  // namespace experiments

#endif /* ifndef EXPERIMENTS_TWM_NRPA */
