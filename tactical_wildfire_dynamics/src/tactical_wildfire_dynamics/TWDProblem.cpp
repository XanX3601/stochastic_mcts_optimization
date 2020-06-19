#include <tactical_wildfire_dynamics/TWDProblem.h>
#include <vector>

TWDProblem::TWDProblem(int grid_width, int grid_height) {
    this->grid_width = grid_width;
    this->grid_height = grid_height;

    int number_of_cell = grid_width * grid_height;

    each_cell_initial_fuel_amount = std::vector<int>(number_of_cell, 0);
    is_each_cell_initially_burning = std::vector<bool>(number_of_cell, false);
    each_ignition_probability = std::vector<std::vector<double>>(number_of_cell, std::vector<double>(number_of_cell, 0));
    each_extinguishment_probability = std::vector<double>(number_of_cell, 0);
    each_cell_burning_reward = std::vector<int>(number_of_cell, -1);
}

TWDProblem::TWDProblem(int grid_width, int grid_height, std::vector<int> each_cell_initial_fuel_amount, std::vector<bool> is_each_cell_initially_burning, std::vector<std::vector<double>> each_ignition_probability, std::vector<double> each_extinguishment_probability, std::vector<int> each_cell_burning_reward) {
    this->grid_width = grid_width;
    this->grid_height = grid_height;
    this->each_cell_initial_fuel_amount = each_cell_initial_fuel_amount;
    this->is_each_cell_initially_burning = is_each_cell_initially_burning;
    this->each_ignition_probability = each_ignition_probability;
    this->each_extinguishment_probability = each_extinguishment_probability;
    this->each_cell_burning_reward = each_cell_burning_reward;
}

TWDProblem::TWDProblem(const TWDProblem& problem) {
    grid_width = problem.grid_width;
    grid_height = problem.grid_height;
    each_cell_initial_fuel_amount = problem.each_cell_initial_fuel_amount;
    is_each_cell_initially_burning = problem.is_each_cell_initially_burning;
    each_ignition_probability = problem.each_ignition_probability;
    each_extinguishment_probability = problem.each_extinguishment_probability;
    each_cell_burning_reward = problem.each_cell_burning_reward;
}

TWDProblem::~TWDProblem() = default;

TWDProblem& TWDProblem::operator=(const TWDProblem& problem) {
    if (this != &problem) {
        grid_width = problem.grid_width;
        grid_height = problem.grid_height;
        each_cell_initial_fuel_amount = problem.each_cell_initial_fuel_amount;
        is_each_cell_initially_burning = problem.is_each_cell_initially_burning;
        each_ignition_probability = problem.each_ignition_probability;
        each_extinguishment_probability = problem.each_extinguishment_probability;
        each_cell_burning_reward = problem.each_cell_burning_reward;
    }

    return *this;
}

int TWDProblem::cell_to_index(const TWDCell& cell) const {
    return cell.y * grid_width + cell.x;
}

TWDCell TWDProblem::index_to_cell(int cell_index) const {
    return TWDCell(cell_index % grid_width, cell_index / grid_width);
}

int TWDProblem::get_cell_initial_fuel_amount(const TWDCell& cell) const {
    return each_cell_initial_fuel_amount[cell_to_index(cell)];
}

bool TWDProblem::is_cell_initially_burning(const TWDCell& cell) const {
    return is_each_cell_initially_burning[cell_to_index(cell)];
}

double TWDProblem::get_probability_that_y_ignites_x(const TWDCell& x, const TWDCell& y) const {
    return each_ignition_probability[cell_to_index(x)][cell_to_index(y)];
}

double TWDProblem::get_probability_to_extinguish_cell(const TWDCell& cell) const {
    return each_extinguishment_probability[cell_to_index(cell)];
}

int TWDProblem::get_cell_burning_reward(const TWDCell& cell) const {
    return each_cell_burning_reward[cell_to_index(cell)];
}
