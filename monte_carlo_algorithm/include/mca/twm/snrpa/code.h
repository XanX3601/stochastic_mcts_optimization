#ifndef MCA_TWM_SNRPA_CODE
#define MCA_TWM_SNRPA_CODE

#include <twm/Action.h>
#include <twm/Board.h>

#include <unordered_set>

namespace mca {
namespace twm {
namespace snrpa {
std::unordered_set<int> compute_all_codes(const ::twm::Problem& problem);
int codify_action(const ::twm::Board& board, const ::twm::Action& action);
::twm::Action decodify_action(const ::twm::Problem& problem, int code);
bool is_code_legal(const ::twm::Board& board, int code);
bool does_code_target_burning_cell(const ::twm::Problem& problem, int code);
int get_target_team_index(const ::twm::Problem& problem, int code);
}  // namespace snrpa
}  // namespace twm
}  // namespace mca

#endif /* ifndef MCA_TWM_SNRPA_CODE */
