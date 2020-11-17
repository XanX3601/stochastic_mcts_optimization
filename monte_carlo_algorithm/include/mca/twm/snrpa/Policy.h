#ifndef MCA_TWM_SNRPA_POLICY
#define MCA_TWM_SNRPA_POLICY
#include <mca/twm/snrpa/Sequence.h>

#include <vector>

namespace mca {
namespace twm {
namespace snrpa {
class Policy {
   private:
    int code_count;
    std::vector<double> each_code_weight;
    double exp_weight_sum;

    void set_weight(int code, double weight);

   public:
    Policy(int code_count);
    void adapt(const Sequence& sequence, double learning_step);
    double get_code_weight(int code) const;
    Sequence generate_sequence() const;
};
}  // namespace snrpa
}  // namespace twm
}  // namespace mca

#endif /* MCA_TWM_SNRPA_POLICY */
