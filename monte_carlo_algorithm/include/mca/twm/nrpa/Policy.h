#ifndef MCA_TWM_NRPA_POLICY
#define MCA_TWM_NRPA_POLICY
#include <mca/twm/nrpa/Sequence.h>

#include <unordered_map>
#include <unordered_set>

namespace mca {
namespace twm {
namespace nrpa {

class Policy {
   private:
    std::unordered_map<int, double> code_to_weight;

    bool does_contain(int code) const;

   public:
    Policy();

    double get_weight(int code) const;
    void set_weight(int code, double weight);
    void adapt(const Sequence& sequence, double learning_step);
};

}  // namespace nrpa
}  // namespace twm
}  // namespace mca

#endif /* MCA_TWM_NRPA_POLICY */
