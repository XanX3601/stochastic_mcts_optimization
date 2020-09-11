#include <experiments/twm/grid.h>

#include <cmath>
#include <vector>

twm::Problem generate_problem_1(int grid_size, int team_count) {
    int grid_width = grid_size;
    int grid_height = grid_size;

    double neighbors_ignition_probability =
        NEIGHBORS_IGNITION_PROBABILITY_GRID_1;

    std::vector<int> each_cell_initial_fuel_amount(
        grid_size * grid_size,
        std::floor(grid_size / (2 * neighbors_ignition_probability)));
    std::vector<bool> is_each_cell_initially_burning(grid_size * grid_size,
                                                     false);
    is_each_cell_initially_burning[grid_size * grid_size - grid_size] = true;
    std::vector<double> each_cell_extinguishment_probability(
        grid_size * grid_size, 0.8);
    std::vector<int> each_cell_burning_reward(grid_size * grid_size);

    int bottom_left_corner_x = 0;
    int bottom_left_corner_y = grid_size - 1;

    int cell_index = 0;
    for (int y = 0; y < grid_size; ++y) {
        for (int x = 0; x < grid_size; ++x) {
            each_cell_burning_reward[cell_index] =
                -1 * (std::abs(x - bottom_left_corner_x) +
                      std::abs(y - bottom_left_corner_y) + 1);
            cell_index++;
        }
    }
    each_cell_burning_reward[grid_size - 1] = -10;

    return twm::Problem(grid_width, grid_height, each_cell_initial_fuel_amount,
                        is_each_cell_initially_burning,
                        neighbors_ignition_probability,
                        each_cell_extinguishment_probability,
                        each_cell_burning_reward, team_count);
}

twm::Board generate_grid_1(twm::Problem* problem) {
    twm::Board board(problem);

    twm::Cell bottom_left_corner(0, problem->get_grid_height() - 1);

    int number_of_free_turn =
        problem->get_cell_initial_fuel_amount(bottom_left_corner);

    for (int turn_index = 0; turn_index < number_of_free_turn; ++turn_index) {
        for (int team_id = 0; team_id < problem->get_team_count(); ++team_id) {
            board = board.get_next_board(twm::Action::null_action);
        }
    }

    board.reset_reward();
    board.scale_fuel_amount(std::pow(problem->get_grid_width(), -.25));

    return board;
}
