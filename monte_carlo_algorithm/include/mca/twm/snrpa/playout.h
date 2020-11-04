#ifndef MCA_TWM_SNRPA_PLAYOUT
#define MCA_TWM_SNRPA_PLAYOUT
#include <mca/twm/snrpa/Policy.h>
#include <mca/twm/snrpa/Sequence.h>
#include <twm/Board.h>

namespace mca {
namespace twm {
namespace snrpa {
Sequence playout(const ::twm::Board& board, const Policy& policy, int playout_count);
int playout(const ::twm::Board& board, const Sequence& sequence);
}  // namespace snrpa
}  // namespace twm
}  // namespace mca
#endif /* ifndef MCA_TWM_SNRPA_PLAYOUT */
