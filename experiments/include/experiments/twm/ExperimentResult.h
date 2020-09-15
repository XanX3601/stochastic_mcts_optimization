#ifndef EXPERIMENTS_TWM_EXPERIMENT_RESULT_H
#define EXPERIMENTS_TWM_EXPERIMENT_RESULT_H

#include <twm/Board.h>
#include <twm/Problem.h>

#include <string>
#include <unordered_map>

namespace experiments {
namespace twm {

class ExperimentResult {
   private:
    ::twm::Problem* problem;
    ::twm::Board* root_board;
    std::unordered_map<std::string, std::string> results_name_to_value;

   public:
    ExperimentResult(::twm::Problem* problem, ::twm::Board* root_board);

    void add_result(std::string result_name, std::string result_value);
    void save_to_file(std::string file_name);
    void close_experiment();
};
}  // namespace twm
}  // namespace experiments

#endif /* EXPERIMENTS_TWM_EXPERIMENT_RESULT_H */
