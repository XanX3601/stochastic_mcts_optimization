#include <tactical_wildfire_dynamics/TWDProblem.h>
#include <vector>

TWDProblem::TWDProblem(int grid_width, int grid_height) {
    this->grid_width = grid_width;
    this->grid_height = grid_height;

    int number_of_cell = grid_width * grid_height;

    each_cell_initial_fuel_amount = std::vector<int>(number_of_cell, 0);
    is_each_cell_initially_burning = std::vector<bool>(number_of_cell, false);
}

TWDProblem::TWDProblem(const TWDProblem& problem) {
    grid_width = problem.grid_width;
    grid_height = problem.grid_height;
    each_cell_initial_fuel_amount = problem.each_cell_initial_fuel_amount;
    is_each_cell_initially_burning = problem.is_each_cell_initially_burning;
}

TWDProblem& TWDProblem::operator=(const TWDProblem& problem) {
    if (this != &problem) {
        grid_width = problem.grid_width;
        grid_height = problem.grid_height;
        each_cell_initial_fuel_amount = problem.each_cell_initial_fuel_amount;
        is_each_cell_initially_burning = problem.is_each_cell_initially_burning;
    }

    return *this;
}

int TWDProblem::cell_to_index(const TWDCell& cell) {
    return cell.y * grid_width + cell.x;
}

TWDCell TWDProblem::index_to_cell(int cell_index) {
    return TWDCell(cell_index % grid_width, cell_index / grid_width);
}
