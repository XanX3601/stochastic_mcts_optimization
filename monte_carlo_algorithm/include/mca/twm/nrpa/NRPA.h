#ifndef MCA_TWM_NRPA_NRPA
#define MCA_TWM_NRPA_NRPA

#include <mca/twm/nrpa/Policy.h>
#include <mca/twm/nrpa/Result.h>
#include <mca/twm/nrpa/Sequence.h>
#include <twm/Board.h>

#include <unordered_set>
#include <vector>

namespace mca {
namespace twm {
namespace nrpa {

class NRPA {
   private:
    ::twm::Board root_board;
    int root_level;
    int number_of_iteration;
    double learning_step;

    Sequence search(int level, const Policy& policy);

   public:
    NRPA(const ::twm::Board& root_board, int root_level, int number_of_iteration,
         double learning_step);

    Result search();
};

}  // namespace nrpa
}  // namespace twm
}  // namespace mca

#endif /* ifndef MCA_TWM_NRPA_NRPA */
