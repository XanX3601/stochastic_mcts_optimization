#include <experiments/grid.h>
#include <cmath>
#include <vector>

twm::Problem generate_problem_1(int grid_size, int team_count) {
    int grid_width = grid_size;
    int grid_height = grid_size;

    double neighbors_ignition_probability = 0.06;

    std::vector<int> each_cell_initial_fuel_amount(grid_size * grid_size, std::floor(grid_size / (2 * neighbors_ignition_probability)));
    std::vector<bool> is_each_cell_initially_burning(grid_size * grid_size, false);
    is_each_cell_initially_burning[grid_size * grid_size - grid_size] = true;
    std::vector<double> each_cell_extinguishment_probability(grid_size * grid_size, 0.8);
    std::vector<int> each_cell_burning_reward(grid_size * grid_size);

    int bottom_left_corner_x = 0;
    int bottom_left_corner_y = grid_size - 1;

    int cell_index = 0;
    for (int y = 0; y < grid_size; ++y) {
        for (int x = 0; x < grid_size; ++x) {
            each_cell_burning_reward[cell_index] = -1 * (std::abs(x - bottom_left_corner_x) + std::abs(y - bottom_left_corner_y) + 1);
            cell_index++;
        }
    }
    each_cell_burning_reward[grid_size - 1] = -10;

    return twm::Problem(grid_width, grid_height, each_cell_initial_fuel_amount, is_each_cell_initially_burning, neighbors_ignition_probability, each_cell_extinguishment_probability, each_cell_burning_reward, team_count);
}
