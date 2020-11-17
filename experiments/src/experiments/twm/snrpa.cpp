#include <experiments/twm/grid.h>
#include <experiments/twm/snrpa.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <mca/twm/snrpa/SNRPA.h>
#include <mca/twm/snrpa/code.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>
#include <twm/Board.h>
#include <twm/Problem.h>

#include <algorithm>
#include <chrono>
#include <execution>
#include <limits>
#include <mutex>
#include <random>
#include <vector>

void experiments::twm::snrpa::solve_problem_1(int grid_size, int team_count, int solve_count,
                                              int root_level, double learning_step,
                                              int iteration_count, int playout_count,
                                              std::string result_file_path,
                                              std::string dir_sequence_path) {
    auto logger = spdlog::stdout_logger_mt("snrpa solve problem 1");
    logger->set_pattern("[%Y-%m-%d %T] [%n] [thread %t] [%l] %v");

    logger->info("Starting");

    auto start = std::chrono::steady_clock::now();

    auto result_file = fmt::output_file(result_file_path);
    std::mutex result_file_lock;

    result_file.print(
        "run_index;run_time;lowest_reward;reward;highest_reward;action_count;team_action_count;"
        "sequence_file_path\n");

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
            std::string solve_sequence_file_path =
                fmt::format("{}/solve{}_sequences.csv", dir_sequence_path, solve_index);

            std::random_device random_device;
            std::mt19937 generator(random_device());

            ::twm::Board board(*p_board);
            while (!board.is_final()) {
                mca::twm::snrpa::SNRPA snrpa(board, root_level, learning_step, iteration_count,
                                             playout_count);

                mca::twm::snrpa::Result result = snrpa.search();
                mca::twm::snrpa::Sequence sequence = result.get_sequence();

                int sequence_index = 0;
                while (!mca::twm::snrpa::is_code_legal(board, sequence.get_code(sequence_index))) {
                    sequence_index++;
                }

                int code = sequence.get_code(sequence_index);
                ::twm::Action action = mca::twm::snrpa::decodify_action(*p_problem, code);

                board = board.get_next_board(action);

                result.save_to_csv(solve_sequence_file_path);

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
                              solve_sequence_file_path);
            solve_done_count++;
            result_file_lock.unlock();

            logger->info("Done solve {} in {} seconds; {} more solve to go", solve_index,
                         solve_duration.count(), solve_count);
        });

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    logger->info("Done in {} seconds", duration.count());
}

void experiments::twm::snrpa::solve_problem_1_with_one_sequence(
    int grid_size, int team_count, int solve_count, int root_level, double learning_step,
    int iteration_count, int playout_count, std::string result_file_path,
    std::string dir_sequence_path) {
    auto logger = spdlog::stdout_logger_mt("snrpa solve problem 1 one sequence");
    logger->set_pattern("[%Y-%m-%d %T] [%n] [thread %t] [%l] %v");

    logger->info("Starting");

    auto start = std::chrono::steady_clock::now();

    auto result_file = fmt::output_file(result_file_path);
    std::mutex result_file_lock;

    result_file.print(
        "run_index;run_time;lowest_reward;reward;highest_reward;action_count;team_action_count;"
        "sequence_file_path\n");

    std::vector<int> solve_indexes(solve_count, 0);
    std::iota(solve_indexes.begin(), solve_indexes.end(), 0);

    std::for_each(
        std::execution::par, solve_indexes.begin(), solve_indexes.end(), [&](int solve_index) {
            logger->info("Starting solve {}", solve_index);

            auto solve_start = std::chrono::steady_clock::now();

            ::twm::Problem* p_problem(generate_problem_1(grid_size, team_count));
            ::twm::Board* p_board(generate_grid_1(p_problem));

            int solve_lowest_reward = p_board->lowest_possible_reward();
            int solve_highest_reward = p_board->highest_possible_reward();
            int solve_action_count = 0;
            std::string solve_sequence_file_path =
                fmt::format("{}/solve{}_sequences.csv", dir_sequence_path, solve_index);

            std::random_device random_device;
            std::mt19937 generator(random_device());

            ::twm::Board board(*p_board);
            mca::twm::snrpa::SNRPA snrpa(board, root_level, learning_step, iteration_count,
                                         playout_count);
            mca::twm::snrpa::Result result = snrpa.search();
            mca::twm::snrpa::Sequence sequence = result.get_sequence();

            while (!board.is_final()) {
                int sequence_index = 0;
                while (!mca::twm::snrpa::is_code_legal(board, sequence.get_code(sequence_index))) {
                    sequence_index++;
                }

                int code = sequence.get_code(sequence_index);
                ::twm::Action action = mca::twm::snrpa::decodify_action(*p_problem, code);

                result.save_to_csv(solve_sequence_file_path);

                board = board.get_next_board(action);

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
                              solve_sequence_file_path);
            result_file_lock.unlock();

            logger->info("Done solve {} in {} seconds", solve_index, solve_duration.count());
        });

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    logger->info("Done in {} seconds", duration.count());
}
