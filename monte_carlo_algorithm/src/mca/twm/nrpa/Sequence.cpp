#include <mca/twm/nrpa/Sequence.h>

mca::twm::nrpa::Sequence::Sequence() {}

int mca::twm::nrpa::Sequence::get_move_count() const { return each_chosen_code.size(); }

int mca::twm::nrpa::Sequence::get_chosen_code(int move_index) const {
    return each_chosen_code[move_index];
}

std::vector<int> const& mca::twm::nrpa::Sequence::get_possible_codes(int move_index) const {
    return each_possible_codes[move_index];
}

void mca::twm::nrpa::Sequence::add_move(int chosen_code, const std::vector<int>& possible_codes) {
    each_chosen_code.push_back(chosen_code);
    each_possible_codes.push_back(possible_codes);
}
