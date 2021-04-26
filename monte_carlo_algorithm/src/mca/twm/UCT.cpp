#include <mca/twm/UCT.h>

#include <cmath>
#include <limits>
#include <random>
#include <vector>

#include "mca/twm/TranspositionEntry.h"
#include "twm/Board.h"

mca::twm::uct::UCT::UCT(double exploration_parameter)
    : exploration_parameter(exploration_parameter) {}

int mca::twm::uct::UCT::playout(const ::twm::Board& board) {
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

int mca::twm::uct::UCT::search(const ::twm::Board& board) {
    if (board.is_final()) {
        return board.get_reward();
    }

    auto legal_actions = board.get_legal_actions();

    if (transposition_table.does_contain(board.get_hash_value())) {
        auto transposition_entry = transposition_table.get_entry(board.get_hash_value());

        int number_of_playout = transposition_entry.get_number_of_playout();

        double best_value = std::numeric_limits<double>::lowest();
        int chosen_action_index;

        for (int action_index = 0; action_index < legal_actions.size(); ++action_index) {
            auto action = legal_actions[action_index];

            double value;

            if (transposition_entry.does_contain(action)) {
                int action_accumulated_reward =
                    transposition_entry.get_action_accumulated_reward(action);
                int action_number_of_playout =
                    transposition_entry.get_action_number_of_playout(action);

                if (action_number_of_playout <= 0) {
                    action_number_of_playout = 4;
                }

                value = (action_accumulated_reward / action_number_of_playout) +
                        exploration_parameter *
                            std::sqrt(std::log(number_of_playout) / action_number_of_playout);
            } else {
                value = std::numeric_limits<double>::max();
            }

            if (value > best_value) {
                best_value = value;
                chosen_action_index = action_index;
            }
        }

        auto next_board = board.get_next_board(legal_actions[chosen_action_index]);
        int reward = search(next_board);
        transposition_table.get_entry(board.get_hash_value())
            .update(legal_actions[chosen_action_index], reward, next_board.get_hash_value());

        return reward;
    } else {
        transposition_table.insert(board.get_hash_value());

        std::random_device random_device;
        std::mt19937 generator(random_device());
        std::uniform_int_distribution<int> distribution(0, legal_actions.size() - 1);

        int random_action_index = distribution(generator);

        auto next_board = board.get_next_board(legal_actions[random_action_index]);
        int reward = playout(next_board);

        transposition_table.get_entry(board.get_hash_value())
            .update(legal_actions[random_action_index], reward, next_board.get_hash_value());

        return reward;
    }
}

mca::twm::TranspositionTable& mca::twm::uct::UCT::get_transposition_table() {
    return transposition_table;
}
