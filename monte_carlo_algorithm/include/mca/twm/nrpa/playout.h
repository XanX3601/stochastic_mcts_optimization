#ifndef MCA_TWM_NRPA_PLAYOUT
#define MCA_TWM_NRPA_PLAYOUT
#include <mca/twm/nrpa/Policy.h>
#include <mca/twm/nrpa/Sequence.h>
#include <twm/Board.h>

namespace mca {
namespace twm {
namespace nrpa {
Sequence playout(const ::twm::Board& board, const Policy& sequence);
}  // namespace nrpa
}  // namespace twm
}  // namespace mca
#endif /* ifndef MCA_TWM_NRPA_PLAYOUT */
