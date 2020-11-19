#include <mca/twm/GRAVE.h>
#include <mca/twm/TranspositionEntry.h>

#include <cmath>
#include <limits>
#include <random>
#include <vector>

mca::twm::grave::GRAVE::GRAVE(double bias, int ref) : bias(bias), ref(ref) {}

std::pair<int, std::unordered_set<twm::Action, mca::twm::action_hash>>
mca::twm::grave::GRAVE::playout(const ::twm::Board& board) {
    if (board.is_final()) {
        return {board.get_reward(), {}};
    }

    std::unordered_set<::twm::Action, action_hash> actions;
    auto legal_actions = board.get_legal_actions();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, legal_actions.size() - 1);

    int action_index = dis(gen);
    actions.insert(legal_actions[action_index]);
    auto next_board = board.get_next_board(legal_actions[action_index]);

    while (!next_board.is_final()) {
        action_index = dis(gen);
        actions.insert(legal_actions[action_index]);
        next_board = next_board.get_next_board(legal_actions[action_index]);
    }

    return {next_board.get_reward(), actions};
}

std::pair<int, std::unordered_set<twm::Action, mca::twm::action_hash>>
mca::twm::grave::GRAVE::search(const ::twm::Board& board, TranspositionEntry& entry_ref) {
    if (board.is_final()) {
        return {board.get_reward(), {}};
    }

    const std::vector<::twm::Action>& actions = board.get_legal_actions();

    if (transposition_table.does_contain(board.get_hash_value())) {
        TranspositionEntry& entry = transposition_table.get_entry(board.get_hash_value());

        if (entry.get_number_of_playout() > ref) {
            entry_ref = entry;
        }

        double best_value = std::numeric_limits<double>::lowest();
        ::twm::Action best_action;

        for (::twm::Action action : actions) {
            double value = std::numeric_limits<double>::max();

            if (entry_ref.does_amaf_contain(action)) {
                double action_reward = 0;
                double action_playout = 0;
                double mean = 0;

                if (entry.does_contain(action)) {
                    action_reward = entry.get_action_accumulated_reward(action);
                    action_playout = entry.get_action_number_of_playout(action);
                    mean = action_reward / action_playout;
                }

                double action_amaf_reward = entry_ref.get_action_accumulated_reward_amaf(action);
                double action_amaf_playout = entry_ref.get_action_number_of_playout_amaf(action);
                double mean_amaf = action_amaf_reward / action_amaf_playout;

                double beta = (action_amaf_playout) / (action_amaf_playout + action_playout +
                                                       bias * action_amaf_playout * action_playout);

                value = (1 - beta) * mean + beta * mean_amaf;
            }

            if (value > best_value) {
                best_value = value;
                best_action = action;
            }
        }

        ::twm::Board next_board = board.get_next_board(best_action);
        auto result = search(next_board, entry_ref);

        transposition_table.get_entry(board.get_hash_value())
            .update(best_action, result.first, next_board.get_hash_value());

        for (::twm::Action action : result.second) {
            transposition_table.get_entry(board.get_hash_value()).update_amaf(action, result.first);
        }

        return result;
    } else {
        transposition_table.insert(board.get_hash_value());

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, actions.size() - 1);

        ::twm::Action random_action = actions[dis(gen)];
        ::twm::Board next_board = board.get_next_board(random_action);

        auto result = playout(next_board);

        transposition_table.get_entry(board.get_hash_value())
            .update(random_action, result.first, next_board.get_hash_value());

        for (::twm::Action action : result.second) {
            transposition_table.get_entry(board.get_hash_value()).update_amaf(action, result.first);
        }

        return result;
    }
}

mca::twm::TranspositionTable& mca::twm::grave::GRAVE::get_transposition_table() {
    return transposition_table;
}
