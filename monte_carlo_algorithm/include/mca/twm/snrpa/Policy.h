#ifndef MCA_TWM_SNRPA_POLICY
#define MCA_TWM_SNRPA_POLICY
#include <mca/twm/snrpa/Sequence.h>

#include <unordered_map>
#include <unordered_set>

namespace mca {
namespace twm {
namespace snrpa {
class Policy {
   private:
    std::unordered_map<int, double> code_to_weight;
    double exp_weight_sum;

    void set_weight(int code, double weight);

   public:
    Policy(const std::unordered_set<int> codes);
    void adapt(const Sequence& sequence, double learning_step);
    double get_code_weight(int code) const;
    Sequence generate_sequence() const;
};
}  // namespace snrpa
}  // namespace twm
}  // namespace mca

#endif /* MCA_TWM_SNRPA_POLICY */