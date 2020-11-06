#ifndef EXPERIMENTS_TWM_SNRPA
#define EXPERIMENTS_TWM_SNRPA

#include <string>

namespace experiments {
namespace twm {
namespace snrpa {
void solve_problem_1(int grid_size, int team_count, int solve_count, int root_level,
                     double learning_step, int iteration_count, int playout_count,
                     std::string result_file_path, std::string dir_sequence_path);
}
}  // namespace twm
}  // namespace experiments

#endif /* ifndef EXPERIMENTS_TWM_SNRPA */
