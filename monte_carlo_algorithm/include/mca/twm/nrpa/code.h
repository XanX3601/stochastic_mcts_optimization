#ifndef MCA_TWM_NRPA_CODE
#define MCA_TWM_NRPA_CODE

#include <twm/Action.h>
#include <twm/Board.h>

#include <unordered_set>

namespace mca {
namespace twm {
namespace nrpa {
int codify_action(const ::twm::Board& board, const ::twm::Action& action);
::twm::Action decodify_action(const ::twm::Problem& problem, int code);
}  // namespace nrpa
}  // namespace twm
}  // namespace mca

#endif /* ifndef MCA_TWM_NRPA_CODE */
