#include "twm/Action.h"
#include <algorithm>
#include <random>
#include <stdexcept>
#include <twm/Problem.h>
#include <unordered_map>
#include <vector>
#include <fmt/core.h>

void twm::Problem::init_legal_actions() {
    int cell_count = get_cell_count();
    legal_actions = std::vector<twm::Action>(cell_count + 1);

    for (int cell_index = 0; cell_index < cell_count; ++cell_index) {
        legal_actions[cell_index] = twm::Action(index_to_cell(cell_index));
    }

    legal_actions[cell_count] = twm::Action::null_action;
}

twm::Problem::Problem(int grid_width, int grid_height) : grid_width(grid_width), grid_height(grid_height) {
    int cell_count = get_cell_count();

    each_cell_initial_fuel_amount = std::vector<int>(cell_count, 0);
    is_each_cell_initially_burning = std::vector<bool>(cell_count, false);
    each_cell_ignition_probabilities = std::vector<std::vector<double>>(cell_count, std::vector<double>(cell_count, 0));
    each_cell_extinguishment_probability = std::vector<double>(cell_count, 0);
    each_cell_burning_reward = std::vector<int>(cell_count, -1);

    init_random_bitstrings();
    init_legal_actions();
}

twm::Problem::Problem(
        int grid_width,
        int grid_height,
        const std::vector<int>& each_cell_initial_fuel_amount,
        const std::vector<bool>& is_each_cell_initially_burning,
        const std::vector<std::vector<double>>& each_cell_ignition_probabilities,
        const std::vector<double>& each_cell_extinguishment_probability,
        const std::vector<int> each_cell_burning_reward,
        int team_count
        ) :
    grid_width(grid_width),
    grid_height(grid_height),
    each_cell_initial_fuel_amount(each_cell_initial_fuel_amount),
    is_each_cell_initially_burning(is_each_cell_initially_burning),
    each_cell_ignition_probabilities(each_cell_ignition_probabilities),
    each_cell_extinguishment_probability(each_cell_extinguishment_probability),
    each_cell_burning_reward(each_cell_burning_reward),
    team_count(team_count) {
        init_random_bitstrings();
        init_legal_actions();
    }

twm::Problem::Problem(
        int grid_width,
        int grid_height,
        const std::vector<int>& each_cell_initial_fuel_amount,
        const std::vector<bool>& is_each_cell_initially_burning,
        double neighbors_ignition_probability,
        const std::vector<double>& each_cell_extinguishment_probability,
        const std::vector<int> each_cell_burning_reward,
        int team_count
        ) :
    grid_width(grid_width),
    grid_height(grid_height),
    each_cell_initial_fuel_amount(each_cell_initial_fuel_amount),
    is_each_cell_initially_burning(is_each_cell_initially_burning),
    each_cell_extinguishment_probability(each_cell_extinguishment_probability),
    each_cell_burning_reward(each_cell_burning_reward),
    team_count(team_count) {
        int cell_count = get_cell_count();
        each_cell_ignition_probabilities = std::vector<std::vector<double>>(cell_count);

        for (int x = 0; x < grid_width; ++x) {
            for (int y = 0; y < grid_height; ++y) {
                twm::Cell cell(x, y);
                int cell_index = cell_to_index(cell);

                each_cell_ignition_probabilities[cell_index] = std::vector<double>(cell_count, 0);

                for (int other_x = 0; other_x < grid_width; ++other_x) {
                    for (int other_y = 0; other_y < grid_height; ++other_y) {
                        twm::Cell other_cell(other_x, other_y);
                        int other_cell_index = cell_to_index(other_cell);

                        if (cell == other_cell) {
                            each_cell_ignition_probabilities[cell_index][other_cell_index] = 1;
                        }
                        else if (cell.is_neighbor(other_cell)) {
                            each_cell_ignition_probabilities[cell_index][other_cell_index] = neighbors_ignition_probability;
                        }
                    }
                }
            }
        }

        init_random_bitstrings();
        init_legal_actions();
    }

int twm::Problem::cell_to_index(const twm::Cell& cell) const {
    return cell.y * grid_width + cell.x;
}

twm::Cell twm::Problem::index_to_cell(int cell_index) const {
    return twm::Cell(cell_index % grid_width, cell_index / grid_width);
}

int twm::Problem::get_grid_width() const {
    return grid_width;
}

int twm::Problem::get_grid_height() const {
    return grid_height;
}

int twm::Problem::get_cell_count() const {
    return grid_width * grid_height;
}

int twm::Problem::get_cell_initial_fuel_amount(const twm::Cell& cell) const {
    return get_cell_initial_fuel_amount(cell_to_index(cell));
}

int twm::Problem::get_cell_initial_fuel_amount(int cell_index) const {
    return each_cell_initial_fuel_amount[cell_index];
}

bool twm::Problem::is_cell_initially_burning(const twm::Cell& cell) const {
    return is_cell_initially_burning(cell_to_index(cell));
}

bool twm::Problem::is_cell_initially_burning(int cell_index) const {
    return is_each_cell_initially_burning[cell_index];
}

double twm::Problem::get_probability_that_y_ignites_x(const twm::Cell& x, const twm::Cell& y) const {
    return get_probability_that_y_ignites_x(cell_to_index(x), cell_to_index(y));
}

double twm::Problem::get_probability_that_y_ignites_x(int x_index, int y_index) const {
    return each_cell_ignition_probabilities[x_index][y_index];
}

double twm::Problem::get_probability_to_extinguish_cell(const twm::Cell& cell) const {
    return get_probability_to_extinguish_cell(cell_to_index(cell));
}

double twm::Problem::get_probability_to_extinguish_cell(int cell_index) const {
    return each_cell_extinguishment_probability[cell_index];
}

int twm::Problem::get_cell_burning_reward(const twm::Cell& cell) const {
    return get_cell_burning_reward(cell_to_index(cell));
}

int twm::Problem::get_cell_burning_reward(int cell_index) const {
    return each_cell_burning_reward[cell_index];
}

int twm::Problem::get_team_count() const {
    return team_count;
}

void twm::Problem::init_random_bitstrings() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long long> dis;

    for (int cell_index = 0; cell_index < get_cell_count(); ++cell_index) {
        each_cell_fuel_random_bitstrings[cell_index] = std::vector<unsigned long long>(get_cell_initial_fuel_amount(cell_index) + 1);
        for (int fuel_amount = 0; fuel_amount < get_cell_initial_fuel_amount(cell_index) + 1; ++fuel_amount) {
            each_cell_fuel_random_bitstrings[cell_index][fuel_amount] = dis(gen);
        }

        each_cell_burning_random_bitstrings[cell_index] = std::vector<unsigned long long>(2);
        for (int is_cell_burning = 0; is_cell_burning < 2; ++is_cell_burning) {
            each_cell_burning_random_bitstrings[cell_index][is_cell_burning] = dis(gen);
        }

        each_cell_team_random_bitstrings[cell_index] = std::vector<unsigned long long>(get_team_count());
        for (int team_index = 0; team_index < get_team_count(); ++team_index) {
            each_cell_team_random_bitstrings[cell_index][team_index] = dis(gen);
        }
    }

    int null_cell_index = cell_to_index(twm::Cell::null_cell);
    each_cell_team_random_bitstrings[null_cell_index] = std::vector<unsigned long long>(get_team_count());
    for (int team_index = 0; team_index < get_team_count(); ++team_index) {
        each_cell_team_random_bitstrings[null_cell_index][team_index] = dis(gen);
    }

    each_team_turn_random_bitstrings = std::vector<unsigned long long>(get_team_count());
    for (int team_index = 0; team_index < get_team_count(); ++team_index) {
        each_team_turn_random_bitstrings[team_index] = dis(gen);
    }
}


unsigned long long twm::Problem::get_cell_fuel_random_bitstring(const twm::Cell& cell, int fuel_amount) const {
    return get_cell_fuel_random_bitstring(cell_to_index(cell), fuel_amount);
}

unsigned long long twm::Problem::get_cell_fuel_random_bitstring(int cell_index, int fuel_amount) const {
    return each_cell_fuel_random_bitstrings.at(cell_index)[fuel_amount];
}

unsigned long long twm::Problem::get_cell_burning_random_bitstring(const twm::Cell& cell, bool is_cell_burning) const {
    return get_cell_burning_random_bitstring(cell_to_index(cell), is_cell_burning);
}

unsigned long long twm::Problem::get_cell_burning_random_bitstring(int cell_index, bool is_cell_burning) const {
    return each_cell_burning_random_bitstrings.at(cell_index)[is_cell_burning];
}

unsigned long long twm::Problem::get_cell_team_random_bitstring(const twm::Cell& cell, int team_index) const {
    return get_cell_team_random_bitstring(cell_to_index(cell), team_index);
}

unsigned long long twm::Problem::get_cell_team_random_bitstring(int cell_index, int team_index) const {
    return each_cell_team_random_bitstrings.at(cell_index)[team_index];
}

unsigned long long twm::Problem::get_team_turn_random_bitstring(int team_index) const {
    return each_team_turn_random_bitstrings[team_index];
}

const std::vector<twm::Action>& twm::Problem::get_legal_actions() const {
    return legal_actions;
}
