#include <experiments/twm/experiments.h>
#include <experiments/twm/grid.h>
#include <mca/twm/UCT.h>
#include <spdlog/spdlog.h>
#include <twm/Action.h>
#include <twm/Board.h>
#include <twm/Problem.h>

#include <chrono>

::experiments::twm::ExperimentResult experiments::twm::experiment_uct_problem_1(
    int grid_size, int team_count, double c, int search_time) {
    spdlog::info("Starting experiment uct problem 1");

    // generate problem
    ::twm::Problem* problem = generate_problem_1(grid_size, team_count);
    ::twm::Board* board = generate_grid_1(problem);

    // play until game is over
    while (!(board->is_final())) {
        // search for action using uct
        mca::twm::UCT uct(*board, c);

        auto start = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed_seconds = now - start;

        while (elapsed_seconds.count() < search_time) {
            uct.search();

            now = std::chrono::steady_clock::now();
            elapsed_seconds = now - start;
        }

        // retrieve best action for each team
        auto transposition_table = uct.get_transposition_table();

        // retrieve entry
        auto entry = transposition_table.get_entry(board.get_hash_value());

        // find most played actions among legal ones
        auto legal_actions = board.get_legal_actions();

        int max_number_of_playout = 0;
        ::twm::Action most_played_action = ::twm::Action::null_action;

        for (auto action : legal_actions) {
            if (entry.does_contain(action)) {
                int number_of_playout =
                    entry.get_action_number_of_playout(action);

                if (number_of_playout > max_number_of_playout) {
                    max_number_of_playout = number_of_playout;
                    most_played_action = action;
                }
            }
        }

        // apply action to board
        board = board.get_next_board(most_played_action);
    }
}
