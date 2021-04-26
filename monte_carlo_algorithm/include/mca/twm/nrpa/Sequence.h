#ifndef MCA_TWM_NRPA_SEQUENCE
#define MCA_TWM_NRPA_SEQUENCE

#include <vector>

namespace mca {
namespace twm {
namespace nrpa {

class Sequence {
   private:
    std::vector<int> each_chosen_code;
    std::vector<std::vector<int>> each_possible_codes;

   public:
    int score;

    Sequence();

    int get_move_count() const;
    int get_chosen_code(int move_index) const;
    std::vector<int> const& get_possible_codes(int move_index) const;
    void add_move(int chosen_code, const std::vector<int>& possible_codes);
};

}  // namespace nrpa
}  // namespace twm
}  // namespace mca

#endif /* MCA_TWM_NRPA_SEQUENCE */
