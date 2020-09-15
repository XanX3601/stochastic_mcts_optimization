#include <experiments/twm/ExperimentResult.h>
#include <fmt/core.h>
#include <fmt/os.h>

#include <fstream>

experiments::twm::ExperimentResult::ExperimentResult(::twm::Problem* problem,
                                                     ::twm::Board* root_board)
    : problem(problem), root_board(root_board) {}

void experiments::twm::ExperimentResult::add_result(std::string result_name,
                                                    std::string result_value) {
    results_name_to_value[result_name] = result_value;
}

void experiments::twm::ExperimentResult::save_to_file(std::string file_name) {
    auto file = fmt::output_file(file_name);

    int max_fuel_amount = 0;
    for (int cell_index = 0; cell_index < problem->get_cell_count();
         ++cell_index) {
        int fuel_amount = root_board->get_cell_fuel_amount(cell_index);
        if (fuel_amount > max_fuel_amount) {
            max_fuel_amount = fuel_amount;
        }
    }
    std::string max_fuel_amount_str = std::to_string(max_fuel_amount);
    int max_fuel_amount_size = max_fuel_amount_str.size();

    file.print("root board fuel amount:\n");

    std::string fuel_amount_format =
        fmt::format("{{:{}d}}", max_fuel_amount_size + 1);

    for (int y = 0; y < problem->get_grid_height(); ++y) {
        for (int x = 0; x < problem->get_grid_width(); ++x) {
            file.print(fuel_amount_format,
                       root_board->get_cell_fuel_amount({x, y}));
        }
        file.print("\n");
    }
}

void experiments::twm::ExperimentResult::close_experiment() {
    delete problem;
    delete root_board;
}
