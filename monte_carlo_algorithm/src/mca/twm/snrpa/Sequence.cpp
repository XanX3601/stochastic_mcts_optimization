#include <mca/twm/snrpa/Sequence.h>

#include <stdexcept>

mca::twm::snrpa::Sequence::Sequence(const std::vector<int> codes)
    : codes(codes), sum_score(0), score_count(0), closed(false) {}

int mca::twm::snrpa::Sequence::get_code(int code_index) const { return codes[code_index]; }

int mca::twm::snrpa::Sequence::get_code_count() const { return codes.size(); }

double mca::twm::snrpa::Sequence::get_mean_score() const { return sum_score / score_count; }

void mca::twm::snrpa::Sequence::add_score(int score) {
    if (closed) {
        throw std::invalid_argument("Sequence is closed and therefore can not be written upon");
    }

    score_count++;
    sum_score += score;
}

void mca::twm::snrpa::Sequence::close() { closed = true; }
