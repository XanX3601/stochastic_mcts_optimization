#include <mca/twm/nrpa/code.h>
#include <mca/twm/nrpa/playout.h>

#include <algorithm>
#include <random>
#include <vector>
mca::twm::nrpa::Sequence mca::twm::nrpa::playout(const ::twm::Board& board, const Policy& policy) {
    Sequence sequence;
    ::twm::Board playout_board(board);

    std::random_device random_device;
    std::mt19937 generator(random_device());

    while (!playout_board.is_final()) {
        auto& legal_action = playout_board.get_legal_actions();

        int action_count = legal_action.size();

        std::vector<int> codes(action_count);
        std::vector<double> cumulative_weights(action_count);

        int code = codify_action(playout_board, legal_action[0]);
        double weight = std::exp(policy.get_weight(code));
        codes[0] = code;
        cumulative_weights[0] = weight;

        for (int action_index = 1; action_index < action_count; ++action_index) {
            code = codify_action(playout_board, legal_action[action_index]);
            weight = std::exp(policy.get_weight(code));

            codes[action_index] = code;
            cumulative_weights[action_index] = cumulative_weights[action_index - 1] + weight;
        }

        std::uniform_real_distribution<double> distribution(0,
                                                            cumulative_weights[action_count - 1]);
        double target = distribution(generator);

        int low = 0;
        int high = action_count;
        bool finished = false;

        while (!finished) {
            int mid = (low + high) / 2;
            double distance = cumulative_weights[mid];

            if (distance < target) {
                low = mid + 1;
            } else if (distance > target) {
                high = mid;
            } else {
                low = mid;
                finished = true;
            }

            finished = finished || low >= high;
        }

        int chosen_action_index = low;
        sequence.add_move(codes[chosen_action_index], codes);
        playout_board = playout_board.get_next_board(legal_action[chosen_action_index]);
    }

    sequence.score = playout_board.get_reward();
    return sequence;
}
