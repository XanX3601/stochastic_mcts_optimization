#include <experiments/twm/ExperimentResult.h>
#include <experiments/twm/experiments.h>
#include <fmt/core.h>

#include <CLI/CLI.hpp>
#include <string>

int main(int argc, const char** argv) {
    CLI::App app;

    std::string problem1 = "problem_1";

    std::string problem = problem1;
    app.add_option("--problem", problem, "the problem to solve", true);

    int grid_size = 8;
    app.add_option("--grid_size", grid_size, "th size of the grid", true);

    int team_count = 4;
    app.add_option("--team_count", team_count, "The number of team", true);

    std::string result_file_path;
    app.add_option("--result_file", result_file_path,
                   "The file in which write the results", true);

    CLI::App* uct_app =
        app.add_subcommand("uct", "Experiment using the UCT algorithm");

    double c = .5;
    uct_app->add_option("-c", c, "hyper parameter c", true);

    int search_time = 60;
    uct_app->add_option("--search_time", search_time,
                        "Iteration search time in second", true);

    uct_app->callback([&]() {
        if (problem == problem1) {
            auto result = experiments::twm::experiment_uct_problem_1(
                grid_size, team_count, c, search_time);
            result.save_to_file("test.txt");
            result.close_experiment();
        } else {
            fmt::print("Unknown problem: {}\n", problem);
        }
    });

    CLI::App* random_app =
        app.add_subcommand("random", "Experiment using the random algorithm");

    random_app->callback([&]() {
        if (problem == problem1) {
        }
        auto result = experiments::twm::experiment_random_problem_1(grid_size,
                                                                    team_count);
        result.save_to_file("")
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
