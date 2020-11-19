#include <experiments/twm/grave.h>
#include <experiments/twm/grid.h>
#include <fmt/os.h>
#include <mca/twm/GRAVE.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>
#include <twm/Board.h>
#include <twm/Problem.h>

#include <algorithm>
#include <chrono>
#include <execution>
#include <mutex>
#include <vector>

void experiments::twm::grave::solve_problem_1(int grid_size, int team_count, int solve_count,
                                              double bias, int ref, double search_time,
                                              std::string result_file_path) {
    auto logger = spdlog::stdout_logger_mt("grave solve problem 1");
    logger->set_pattern("[%Y-%m-%d %T] [%n] [thread %t] [%l] %v");

    logger->info("Starting");

    auto start = std::chrono::steady_clock::now();

    auto result_file = fmt::output_file(result_file_path);
    std::mutex result_file_lock;

    result_file.print(
        "run_index;run_time;lowest_reward;reward;highest_reward;action_count;team_action_count;"
        "grave_"
        "search_count\n");

    std::vector<int> solve_indexes(solve_count, 0);
    std::iota(solve_indexes.begin(), solve_indexes.end(), 0);

    int solve_done_count = 0;

    std::for_each(
        std::execution::par, solve_indexes.begin(), solve_indexes.end(), [&](int solve_index) {
            logger->info("Starting solve {}", solve_index);

            auto solve_start = std::chrono::steady_clock::now();

            ::twm::Problem* p_problem(generate_problem_1(grid_size, team_count));
            ::twm::Board* p_board(generate_grid_1(p_problem));

            int solve_lowest_reward = p_board->lowest_possible_reward();
            int solve_highest_reward = p_board->highest_possible_reward();
            int solve_action_count = 0;
            int solve_grave_search_count = 0;

            ::twm::Board board(*p_board);
            while (!board.is_final()) {
                ::mca::twm::grave::GRAVE grave(bias, ref);

                auto search_start = std::chrono::steady_clock::now();

                while (
                    std::chrono::duration<double>(std::chrono::steady_clock::now() - search_start)
                        .count() < search_time) {
                    mca::twm::TranspositionEntry entry(-1);
                    grave.search(board, entry);
                    solve_grave_search_count++;
                }

                mca::twm::TranspositionEntry& transition_entry =
                    grave.get_transposition_table().get_entry(board.get_hash_value());
                auto legal_actions = board.get_legal_actions();
                ::twm::Action most_played_action = ::twm::Action::null_action;
                int max_playout_count = std::numeric_limits<int>::lowest();

                for (auto action : legal_actions) {
                    int playout_count = 0;

                    if (transition_entry.does_contain(action)) {
                        playout_count = transition_entry.get_action_number_of_playout(action);
                    }

                    if (playout_count > max_playout_count) {
                        max_playout_count = playout_count;
                        most_played_action = action;
                    }
                }

                board = board.get_next_board(most_played_action);
                solve_action_count++;

                logger->info("Solve {}: {} burning cell left; {} action played", solve_index,
                             board.get_burning_cell_count(), solve_action_count);
            }

            int solve_reward = board.get_reward();
            std::chrono::duration<double> solve_duration(std::chrono::steady_clock::now() -
                                                         solve_start);

            result_file_lock.lock();
            result_file.print("{};{};{};{};{};{};{};{}\n", solve_index, solve_duration.count(),
                              solve_lowest_reward, solve_reward, solve_highest_reward,
                              solve_action_count, solve_action_count / team_count,
                              solve_grave_search_count);
            solve_done_count++;
            result_file_lock.unlock();

            logger->info("Done solve {} in {} seconds; {} more solve to go", solve_index,
                         solve_duration.count(), solve_count - solve_done_count);
        });

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    logger->info("Done in {} seconds", duration.count());
}
