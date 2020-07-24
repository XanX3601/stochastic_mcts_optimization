#include "mca/twm/TranspositionEntry.h"
#include "twm/Board.h"
#include <cmath>
#include <limits>
#include <mca/twm/UCT.h>
#include <random>
#include <vector>

int mca::twm::UCT::playout(const ::twm::Board& board) {
    if (board.is_final()) {
        return board.get_reward();
    }

    auto legal_actions = board.get_legal_actions();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, legal_actions.size() - 1);

    auto next_board = board.get_next_board(legal_actions[dis(gen)]);

    while (!next_board.is_final()) {
        next_board = next_board.get_next_board(legal_actions[dis(gen)]);
    }

    return next_board.get_reward();
}

mca::twm::UCT::UCT(::twm::Board board, double c): root_board(board), c(c) {
}

void mca::twm::UCT::search() {
    search(root_board);
}

int mca::twm::UCT::search(const ::twm::Board& board) {
    if (board.is_final()) {
        return board.get_reward();
    }

    auto legal_actions = board.get_legal_actions();

    if (transposition_table.does_contain(board.get_hash_value())) {
        auto transposition_entry = transposition_table.get_entry(board.get_hash_value());

        int number_of_playout = transposition_entry.get_number_of_playout();

        double best_value = std::numeric_limits<double>::min();
        int chosen_action_index;

        for (int action_index = 0; action_index < legal_actions.size(); ++action_index) {
            auto action = legal_actions[action_index];

            double value;

            if (transposition_entry.does_contain(action)) {
                int action_accumulated_reward = transposition_entry.get_action_accumulated_reward(action);
                int action_number_of_playout = transposition_entry.get_action_number_of_playout(action);

                value = (action_accumulated_reward / action_number_of_playout) + c * std::sqrt(std::log(number_of_playout) / action_number_of_playout);
            }
            else {
                value = best_value + 1;
            }

            if (value > best_value) {
                best_value = value;
                chosen_action_index = action_index;
            }
        }

        auto next_board = board.get_next_board(legal_actions[chosen_action_index]);
        int reward = search(next_board);
        transposition_entry.update(legal_actions[chosen_action_index], reward);

        return reward;
    }
    else {
        transposition_table.insert(board.get_hash_value());

        std::random_device random_device;
        std::mt19937 generator(random_device());
        std::uniform_int_distribution<int> distribution(0, legal_actions.size() - 1);

        int random_action_index = distribution(generator);

        int reward = playout(board.get_next_board(legal_actions[random_action_index]));

        transposition_table.get_entry(board.get_hash_value()).update(legal_actions[random_action_index], reward);

        return reward;
    }
}

::mca::twm::TranspositionTable& ::mca::twm::UCT::get_transposition_table() {
    return transposition_table;
}
