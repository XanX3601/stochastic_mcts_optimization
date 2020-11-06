#include <mca/twm/nrpa/code.h>
#include <mca/twm/nrpa/playout.h>

#include <random>
mca::twm::nrpa::Sequence mca::twm::nrpa::playout(const ::twm::Board& board, const Policy& policy) {
    Sequence sequence;
    ::twm::Board playout_board(board);

    std::random_device random_device;
    std::mt19937 generator(random_device());

    while (!playout_board.is_final()) {
        auto legal_actions = playout_board.get_legal_actions();
        std::vector<double> weights;
        std::vector<int> possible_codes;
        double sum_weight = 0;

        for (auto action : legal_actions) {
            int code = codify_action(playout_board, action);
            double weight = std::exp(policy.get_weight(code));

            weights.push_back(weight);
            possible_codes.push_back(code);
            sum_weight += weight;
        }

        std::uniform_real_distribution<> distribution(0, sum_weight);
        double target = distribution(generator) - weights[0];

        int chosen_code_index = 0;
        while (target > 0) {
            chosen_code_index++;
            target -= weights[chosen_code_index];
        }

        playout_board = playout_board.get_next_board(legal_actions[chosen_code_index]);
        sequence.add_move(possible_codes[chosen_code_index], possible_codes);
    }

    sequence.score = playout_board.get_reward();
    return sequence;
}
