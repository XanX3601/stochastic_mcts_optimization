#include <mca/twm/nrpa/NRPA.h>
#include <mca/twm/nrpa/code.h>
#include <mca/twm/nrpa/playout.h>

#include <limits>

mca::twm::nrpa::NRPA::NRPA(const ::twm::Board& root_board, int root_level, int number_of_iteration,
                           double learning_step)
    : root_board(root_board),
      root_level(root_level),
      number_of_iteration(number_of_iteration),
      learning_step(learning_step) {}

mca::twm::nrpa::Result mca::twm::nrpa::NRPA::search() { return {search(root_level, {})}; }

mca::twm::nrpa::Sequence mca::twm::nrpa::NRPA::search(int level, const Policy& policy) {
    if (level == 0) {
        return playout(root_board, policy);
    }

    Policy search_policy(policy);

    int best_score = std::numeric_limits<int>::lowest();
    Sequence best_sequence;

    for (int iteration_index = 0; iteration_index < number_of_iteration; ++iteration_index) {
        Sequence sequence = search(level - 1, search_policy);

        if (sequence.score > best_score) {
            best_sequence = sequence;
            best_score = sequence.score;
        }

        search_policy.adapt(best_sequence, learning_step);
    }

    return best_sequence;
}
