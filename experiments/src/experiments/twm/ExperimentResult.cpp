#include <experiments/twm/ExperimentResult.h>
#include <fmt/core.h>
#include <fmt/os.h>

#include <fstream>

experiments::twm::ExperimentResult::ExperimentResult(::twm::Problem* problem,
                                                     ::twm::Board* root_board)
    : problem(problem), root_board(root_board) {}

void experiments::twm::ExperimentResult::add_result(std::string result_name,
                                                    std::string result_value) {
    each_result_name.push_back(result_name);
    each_result_value.push_back(result_value);
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

    // print results name to value
    for (int result_index = 0; result_index < each_result_name.size();
         ++result_index) {
        file.print("{}: {}\n", each_result_name[result_index],
                   each_result_value[result_index]);
    }
    file.print("\n");

    // print root board
    file.print("ROOT BOARD:\n");

    // print fuel amount as a grid
    file.print("  fuel amount:\n");

    std::string fuel_amount_format =
        fmt::format("{{:{}d}}", max_fuel_amount_size + 1);

    for (int y = 0; y < problem->get_grid_height(); ++y) {
        file.print("  ");
        for (int x = 0; x < problem->get_grid_width(); ++x) {
            file.print(fuel_amount_format,
                       root_board->get_cell_fuel_amount({x, y}));
        }
        file.print("\n");
    }
    file.print("\n");

    // print burnign cells as a grid
    file.print("  burning cells:\n");

    for (int y = 0; y < problem->get_grid_height(); ++y) {
        file.print("  ");
        for (int x = 0; x < problem->get_grid_width(); ++x) {
            file.print("{} ", root_board->is_cell_burning({x, y}) ? "X" : "_");
        }
        file.print("\n");
    }
    file.print("\n");

    // print burning cell count
    file.print("  burning cell count: {}\n\n",
               root_board->get_burning_cell_count());
}

void experiments::twm::ExperimentResult::close_experiment() {
    delete problem;
    delete root_board;
}
