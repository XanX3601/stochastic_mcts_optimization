#include <experiments/twm/ExperimentResult.h>
#include <experiments/twm/experiments.h>
#include <experiments/twm/grid.h>
#include <fmt/core.h>

#include <CLI/CLI.hpp>
#include <set>
#include <string>

int main(int argc, const char** argv) {
    CLI::App app;

    std::string problem1 = "problem_1";

    std::set<std::string> problems = {problem1};

    std::string problem = problem1;
    app.add_option("--problem", problem, "the problem to solve", true)
        ->check(CLI::IsMember(problems));

    int grid_size = 8;
    app.add_option("--grid-size", grid_size, "the size of the grid", true);

    int team_count = 4;
    app.add_option("--team-count", team_count, "the number of team", true);

    std::string result_file = "result";
    app.add_option("--result-file", result_file,
                   "the file in which write the result experiment", true);

    CLI::App* uct_app =
        app.add_subcommand("uct", "Experiment using the UCT algorithm");

    double c = .5;
    uct_app->add_option("-c", c, "hyper parameter c", true);

    int search_time = 60;
    uct_app->add_option("--search-time", search_time,
                        "search time per iteration in seconds", true);

    uct_app->callback([&]() {
        if (problem == problem1) {
            auto result = experiments::twm::experiment_uct_problem_1(
                grid_size, team_count, c, search_time);
            result.save_to_file(result_file);
            result.close_experiment();
        } else {
            fmt::print("Unknown problem: {}\n", problem);
        }
    });

    CLI::App* random_app =
        app.add_subcommand("random", "Experiment using the random algorithm");

    random_app->callback([&]() {
        if (problem == problem1) {
            auto result = experiments::twm::experiment_random_problem_1(
                grid_size, team_count);
            result.save_to_file(result_file);
            result.close_experiment();
        } else {
            fmt::print("Unknown problem: {}\n", problem);
        }
    });

    CLI::App* testing_app = app.add_subcommand("test", "Just testing stuff");

    testing_app->callback([&]() {
        twm::Problem* problem = experiments::twm::generate_problem_1(4, 4);
        twm::Board* board = experiments::twm::generate_grid_1(problem);
        board->highest_possible_reward();
        delete board;
        delete problem;
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
