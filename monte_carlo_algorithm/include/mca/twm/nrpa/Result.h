#ifndef MCA_TWM_NRPA_RESULT
#define MCA_TWM_NRPA_RESULT

#include <mca/twm/nrpa/Sequence.h>

namespace mca {
namespace twm {
namespace nrpa {

class Result {
   private:
    Sequence sequence;

   public:
    Result(const Sequence& sequence);

    Sequence get_sequence() const;
};

}  // namespace nrpa
}  // namespace twm
}  // namespace mca

#endif /* MCA_TWM_NRPA_RESULT */
