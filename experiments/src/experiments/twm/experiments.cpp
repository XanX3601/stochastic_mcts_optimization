#include <experiments/twm/experiments.h>
#include <experiments/twm/grid.h>
#include <mca/twm/UCT.h>
#include <spdlog/spdlog.h>
#include <twm/Action.h>
#include <twm/Board.h>
#include <twm/Problem.h>

#include <chrono>
#include <random>

::experiments::twm::ExperimentResult experiments::twm::experiment_uct_problem_1(int grid_size,
                                                                                int team_count,
                                                                                double c,
                                                                                int search_time) {
    auto start_experiment = std::chrono::steady_clock::now();

    spdlog::info("{} {} {} start", EXPERIMENT_TAG, UCT_TAG, PROBLEM_1_TAG);

    // generate problem
    ::twm::Problem* problem = generate_problem_1(grid_size, team_count);
    ::twm::Board* root_board = generate_grid_1(problem);

    ::twm::Board board = *root_board;

    // prepare results container
    ::experiments::twm::ExperimentResult results(problem, root_board);

    results.add_result("problem", "problem 1");
    results.add_result("algorithm", "uct");
    results.add_result("grid size", std::to_string(grid_size));
    results.add_result("team count", std::to_string(team_count));
    results.add_result("c", std::to_string(c));
    results.add_result("search time (in seconds)", std::to_string(search_time));
    results.add_result("lowest reward", std::to_string(root_board->lowest_possible_reward()));
    results.add_result("highest reward", std::to_string(root_board->highest_possible_reward()));

    int action_count = 0;
    int uct_search_count = 0;
    int next_hash_value_count = 0;

    // play until game is over
    while (!(board.is_final())) {
        // search for action using uct
        auto start = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed_seconds = now - start;

        mca::twm::TranspositionTable transposition_table;

        while (elapsed_seconds.count() < search_time) {
            mca::twm::uct(board, transposition_table, c);
            uct_search_count++;

            now = std::chrono::steady_clock::now();
            elapsed_seconds = now - start;
        }

        // retrieve entry
        auto entry = transposition_table.get_entry(board.get_hash_value());

        // find most played actions among legal ones
        auto legal_actions = board.get_legal_actions();

        int max_number_of_playout = 0;
        ::twm::Action most_played_action = ::twm::Action::null_action;

        for (auto action : legal_actions) {
            if (entry.does_contain(action)) {
                int number_of_playout = entry.get_action_number_of_playout(action);

                if (number_of_playout > max_number_of_playout) {
                    max_number_of_playout = number_of_playout;
                    most_played_action = action;
                }
            }
        }

        // apply action to board
        board = board.get_next_board(most_played_action);
        next_hash_value_count += entry.get_action_number_of_unique_hash_value(most_played_action);
        spdlog::info("{} {} {} played action {}", EXPERIMENT_TAG, UCT_TAG, PROBLEM_1_TAG,
                     action_count);
        action_count++;
    }

    spdlog::info("{} {} {} done", EXPERIMENT_TAG, UCT_TAG, PROBLEM_1_TAG);

    auto now = std::chrono::steady_clock::now();

    std::chrono::duration<double> experiment_duration = now - start_experiment;
    results.add_result("experiment time (in seconds)", std::to_string(experiment_duration.count()));

    results.add_result("number of action played", std::to_string(action_count));
    results.add_result("number of action played by team",
                       std::to_string(action_count / problem->get_team_count()));
    results.add_result("number of uct search", std::to_string(uct_search_count));
    results.add_result("mean number of uct search",
                       std::to_string(uct_search_count / action_count));
    results.add_result("number of unique next hash value", std::to_string(next_hash_value_count));
    results.add_result("mean number of unique next hash value",
                       std::to_string(next_hash_value_count / action_count));
    results.add_result("final reward", std::to_string(board.get_reward()));

    return results;
}

::experiments::twm::ExperimentResult experiments::twm::experiment_random_problem_1(int grid_size,
                                                                                   int team_count) {
    auto start_experiment = std::chrono::steady_clock::now();

    spdlog::info("{} {} {} start", EXPERIMENT_TAG, RANDOM_TAG, PROBLEM_1_TAG);

    // generate problem
    ::twm::Problem* problem = generate_problem_1(grid_size, team_count);
    ::twm::Board* root_board = generate_grid_1(problem);

    ::twm::Board board = *root_board;

    // prepare results container
    ::experiments::twm::ExperimentResult results(problem, root_board);

    results.add_result("problem", "problem 1");
    results.add_result("algorithm", "random");
    results.add_result("grid size", std::to_string(grid_size));
    results.add_result("team count", std::to_string(team_count));
    results.add_result("lowest reward", std::to_string(root_board->lowest_possible_reward()));
    results.add_result("highest reward", std::to_string(root_board->highest_possible_reward()));

    int action_count = 0;

    std::random_device random_device;
    std::mt19937 generator(random_device());

    // play until game is over
    while (!(board.is_final())) {
        // retieve legal actions
        auto legal_actions = board.get_legal_actions();

        // choose uniformely an action among legal ones
        std::uniform_int_distribution<> distribution(0, legal_actions.size() - 1);

        // apply action to board
        board = board.get_next_board(legal_actions[distribution(generator)]);
        spdlog::info("{} {} {} played action {}", EXPERIMENT_TAG, RANDOM_TAG, PROBLEM_1_TAG,
                     action_count);
        action_count++;
    }

    spdlog::info("{} {} {} done", EXPERIMENT_TAG, RANDOM_TAG, PROBLEM_1_TAG);

    auto now = std::chrono::steady_clock::now();

    std::chrono::duration<double> experiment_duration = now - start_experiment;
    results.add_result("experiment time (in seconds)", std::to_string(experiment_duration.count()));

    results.add_result("number of action played", std::to_string(action_count));
    results.add_result("number of action played by team",
                       std::to_string(action_count / problem->get_team_count()));
    results.add_result("final reward", std::to_string(board.get_reward()));

    return results;
}
