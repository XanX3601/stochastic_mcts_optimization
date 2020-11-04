#include <mca/twm/snrpa/SNRPA.h>
#include <mca/twm/snrpa/code.h>
#include <mca/twm/snrpa/playout.h>

#include <limits>

mca::twm::snrpa::SNRPA::SNRPA(const ::twm::Board& root_board, int root_level, double learning_step,
                              int iteration_count, int playout_count)
    : root_board(root_board),
      root_level(root_level),
      learning_step(learning_step),
      iteration_count(iteration_count),
      playout_count(playout_count) {}

mca::twm::snrpa::Result mca::twm::snrpa::SNRPA::search() const {
    Result result;

    Sequence sequence = search(root_level, compute_all_codes(root_board.get_problem()), result);

    result.set_sequence(sequence);
    result.close();

    return result;
}

mca::twm::snrpa::Sequence mca::twm::snrpa::SNRPA::search(int level, const Policy& policy,
                                                         Result& result) const {
    if (level == 0) {
        return playout(root_board, policy, playout_count);
    }

    Sequence best_sequence({});
    best_sequence.add_score(std::numeric_limits<int>::lowest());
    best_sequence.close();

    Policy policy_copy(policy);

    for (int iteration_index = 0; iteration_index < iteration_count; ++iteration_index) {
        Sequence sequence = search(level - 1, policy_copy, result);

        if (sequence.get_mean_score() > best_sequence.get_mean_score()) {
            best_sequence = sequence;

            if (level == root_level) {
                result.add_root_sequence(best_sequence);
            }
        }

        policy_copy.adapt(best_sequence, learning_step);
    }

    return best_sequence;
}
