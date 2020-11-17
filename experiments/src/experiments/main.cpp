#include <experiments/twm/nrpa.h>
#include <experiments/twm/random.h>
#include <experiments/twm/snrpa.h>
#include <experiments/twm/uct.h>

#include <CLI/CLI.hpp>
#include <string>

int main(int argc, const char** argv) {
    CLI::App app;

    CLI::App* uct_app = app.add_subcommand("uct");

    int uct_grid_size;
    uct_app->add_option("grid_size", uct_grid_size)->required();

    int uct_team_count;
    uct_app->add_option("team_count", uct_team_count)->required();

    double uct_exploration_parameter = .5;
    uct_app->add_option("--exploration_parameter,--ep", uct_exploration_parameter,
                        "exploration parameter", true);

    double uct_search_time = 60;
    uct_app->add_option("--search-time,--st", uct_search_time, "search time", true);

    int uct_solve_count = 1;
    uct_app->add_option("--solve-count,--sc", uct_solve_count, "solve count", true);

    std::string uct_result_file_path = "uct_result.csv";
    uct_app->add_option("--result-file,--rf", uct_result_file_path, "result file path", true);

    uct_app->callback([&]() {
        experiments::twm::uct::solve_problem_1(uct_grid_size, uct_team_count, uct_solve_count,
                                               uct_exploration_parameter, uct_search_time,
                                               uct_result_file_path);
    });

    CLI::App* random_app = app.add_subcommand("random");

    int random_grid_size;
    random_app->add_option("grid_size", random_grid_size)->required();

    int random_team_count;
    random_app->add_option("team_count", random_team_count)->required();

    int random_solve_count = 1;
    random_app->add_option("--solve-count,--sc", random_solve_count, "solve count", true);

    std::string random_result_file = "random_result.csv";
    random_app->add_option("--result-file,--rf", random_result_file, "result file path", true);

    random_app->callback([&]() {
        experiments::twm::random::solve_problem_1(random_grid_size, random_team_count,
                                                  random_solve_count, random_result_file);
    });

    CLI::App* nrpa_app = app.add_subcommand("nrpa");

    int nrpa_grid_size;
    nrpa_app->add_option("grid_size", nrpa_grid_size)->required();

    int nrpa_team_count;
    nrpa_app->add_option("team_count", nrpa_team_count)->required();

    int nrpa_solve_count = 1;
    nrpa_app->add_option("--solve-count,--sc", nrpa_solve_count, "solve count", true);

    int nrpa_root_level = 1;
    nrpa_app->add_option("--root-level,--rl", nrpa_root_level, "root level", true);

    int nrpa_iteration_count = 100;
    nrpa_app->add_option("--iteration-count,--ic", nrpa_iteration_count, "iteration count", true);

    double nrpa_learning_step = 1;
    nrpa_app->add_option("--learning-step,--ls", nrpa_learning_step, "learning step", true);

    std::string nrpa_result_file_path = "nrpa_result.csv";
    nrpa_app->add_option("--result-file,--rf", nrpa_result_file_path, "result file path", true);

    nrpa_app->callback([&]() {
        experiments::twm::nrpa::solve_problem_1(nrpa_grid_size, nrpa_team_count, nrpa_solve_count,
                                                nrpa_root_level, nrpa_iteration_count,
                                                nrpa_learning_step, nrpa_result_file_path);
    });

    CLI::App* snrpa_app = app.add_subcommand("snrpa");

    int snrpa_grid_size;
    snrpa_app->add_option("grid_size", snrpa_grid_size)->required();

    int snrpa_team_count;
    snrpa_app->add_option("team_count", snrpa_team_count)->required();

    int snrpa_solve_count = 1;
    snrpa_app->add_option("--solve-count,--sc", snrpa_solve_count, "solve count", true);

    int snrpa_root_level = 1;
    snrpa_app->add_option("--root-level,--rl", snrpa_root_level, "root level", true);

    double snrpa_learning_step = 1;
    snrpa_app->add_option("--learning-step,--ls", snrpa_learning_step, "learning step", true);

    int snrpa_iteration_count = 100;
    snrpa_app->add_option("--iteration-count,--ic", snrpa_iteration_count, "iteration count", true);

    int snrpa_playout_count = 100;
    snrpa_app->add_option("--playout-count,--pc", snrpa_playout_count, "playout count", true);

    std::string snrpa_result_file_path = "snrpa_result.csv";
    snrpa_app->add_option("--result-file,--rf", snrpa_result_file_path, "result file path", true);

    std::string snrpa_dir_sequence_path = "./";
    snrpa_app->add_option("--sequence-dir,--sd", snrpa_dir_sequence_path, "sequence directory path",
                          true);

    bool snrpa_use_only_one_sequence = false;
    snrpa_app->add_flag("--one-sequence,--os", snrpa_use_only_one_sequence,
                        "use only one sequence");

    snrpa_app->callback([&]() {
        if (snrpa_use_only_one_sequence) {
            experiments::twm::snrpa::solve_problem_1_with_one_sequence(
                snrpa_grid_size, snrpa_team_count, snrpa_solve_count, snrpa_root_level,
                snrpa_learning_step, snrpa_iteration_count, snrpa_playout_count,
                snrpa_result_file_path, snrpa_dir_sequence_path);
        } else {
            experiments::twm::snrpa::solve_problem_1(
                snrpa_grid_size, snrpa_team_count, snrpa_solve_count, snrpa_root_level,
                snrpa_learning_step, snrpa_iteration_count, snrpa_playout_count,
                snrpa_result_file_path, snrpa_dir_sequence_path);
        }
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
