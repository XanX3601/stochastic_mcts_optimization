#ifndef MCA_TWM_SNRPA_SEQUENCE
#define MCA_TWM_SNRPA_SEQUENCE

#include <vector>

namespace mca {
namespace twm {
namespace snrpa {

class Sequence {
   private:
    std::vector<int> codes;
    double sum_score;
    int score_count;
    bool closed;

   public:
    Sequence(const std::vector<int> codes);
    int get_code(int code_index) const;
    int get_code_count() const;
    double get_mean_score() const;
    void add_score(int score);
    void close();
};

}  // namespace snrpa
}  // namespace twm
}  // namespace mca

#endif /* MCA_TWM_SNRPA_SEQUENCE */
