#include <mca/twm/nrpa/Result.h>

mca::twm::nrpa::Result::Result(const mca::twm::nrpa::Sequence& sequence) : sequence(sequence) {}

mca::twm::nrpa::Sequence mca::twm::nrpa::Result::get_sequence() const { return sequence; }
