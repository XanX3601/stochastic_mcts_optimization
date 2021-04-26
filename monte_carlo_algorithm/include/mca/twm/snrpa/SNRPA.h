#ifndef MCA_TWM_SNRPA
#define MCA_TWM_SNRPA

#include <mca/twm/snrpa/Policy.h>
#include <mca/twm/snrpa/Result.h>
#include <mca/twm/snrpa/Sequence.h>
#include <twm/Board.h>

#include <unordered_set>
#include <vector>

namespace mca {
namespace twm {
namespace snrpa {
class SNRPA {
   private:
    int root_level;
    double learning_step;
    int iteration_count;
    int playout_count;

    ::twm::Board root_board;

    Sequence search(int level, const Policy& policy, Result& result) const;

   public:
    SNRPA(const ::twm::Board& root_board, int root_level, double learning_step, int iteration_count,
          int playout_count);
    Result search() const;
};
}  // namespace snrpa
}  // namespace twm
}  // namespace mca

#endif /* ifndef MCA_TWM_SNRPA */
