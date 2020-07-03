#include <random>
#include <stdexcept>
#include <tactical_wildfire_dynamics/Board.h>
#include <vector>

TWD::Board::Board(TWD::Problem* problem) : problem(problem) {
    int cell_count = problem->get_cell_count();

    each_cell_fuel_amount = std::vector<int>(cell_count);
    is_each_cell_burning = std::vector<bool>(cell_count);

    reward = 0;
    burning_cell_count = 0;
    next_team_index_to_play = 0;
    hash_value = 0;

    hash_value ^= problem->get_team_turn_random_bitstring(0);

    for (int cell_index = 0; cell_index < problem->get_cell_count(); ++cell_index) {
        int fuel_amount = problem->get_cell_initial_fuel_amount(cell_index);
        each_cell_fuel_amount[cell_index] = fuel_amount;
        hash_value ^= problem->get_cell_fuel_random_bitstring(cell_index, fuel_amount);

        bool is_cell_burning = problem->is_cell_initially_burning(cell_index);
        is_each_cell_burning[cell_index] = is_cell_burning;

        if (is_cell_burning) {
            reward += problem->get_cell_burning_reward(cell_index);
            burning_cell_count++;
        }
        hash_value ^= problem->get_cell_burning_random_bitstring(cell_index, is_cell_burning);
    }

    each_team_cell = std::vector<TWD::Cell>(problem->get_team_count(), {0, 0});
    for (int team_index = 0; team_index < problem->get_team_count(); ++team_index) {
        hash_value ^= problem->get_cell_team_random_bitstring({0, 0}, team_index);
    }
}

int TWD::Board::get_cell_fuel_amount(const TWD::Cell& cell) const {
    return get_cell_fuel_amount(problem->cell_to_index(cell));
}

int TWD::Board::get_cell_fuel_amount(int cell_index) const {
    return each_cell_fuel_amount[cell_index];
}

bool TWD::Board::is_cell_burning(const TWD::Cell& cell) const {
    return is_cell_burning(problem->cell_to_index(cell));
}

bool TWD::Board::is_cell_burning(int cell_index) const {
    return is_each_cell_burning[cell_index];
}

TWD::Cell TWD::Board::get_team_cell(int team_index) const {
    return each_team_cell[team_index];
}

int TWD::Board::get_next_team_index_to_play() const {
    return next_team_index_to_play;
}

int TWD::Board::get_reward() const {
    return reward;
}

bool TWD::Board::is_action_legal(const TWD::Action& action) const {
    return 0 <= action.cell.x && action.cell.x < problem->get_grid_width() && 0 <= action.cell.y && action.cell.y < problem->get_grid_height();
}

TWD::Board TWD::Board::get_next_board(const TWD::Action& action) const {
    if (!(is_action_legal(action))) {
        throw std::invalid_argument("illegal action received");
    }

    TWD::Board next_board(*this);

    next_board.hash_value ^= problem->get_cell_team_random_bitstring(get_team_cell(next_team_index_to_play), next_team_index_to_play);
    next_board.hash_value ^= problem->get_cell_team_random_bitstring(action.cell, next_team_index_to_play);
    next_board.each_team_cell[next_team_index_to_play] = action.cell;
    next_board.next_team_index_to_play++;

    if (next_board.get_next_team_index_to_play() == problem->get_team_count()) {
        next_board.next_team_index_to_play = 0;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0, 1);

        for (int cell_index = 0; cell_index < problem->get_cell_count(); ++cell_index) {
            if (is_cell_burning(cell_index)) {
                next_board.reward += problem->get_cell_burning_reward(cell_index);

                double rho_2 = 1;

                if (get_cell_fuel_amount(cell_index) > 0) {
                    int cell_fuel_amount = get_cell_fuel_amount(cell_index);
                    next_board.hash_value ^= problem->get_cell_fuel_random_bitstring(cell_index, cell_fuel_amount);
                    next_board.hash_value ^= problem->get_cell_fuel_random_bitstring(cell_index, cell_fuel_amount - 1);
                    next_board.each_cell_fuel_amount[cell_index] = cell_fuel_amount - 1;

                    TWD::Cell cell = problem->index_to_cell(cell_index);
                    for (int team_index = 0; team_index < problem->get_team_count(); ++team_index) {
                        if (get_team_cell(team_index) == cell) {
                            rho_2 *= (1 - problem->get_probability_to_extinguish_cell(cell_index));
                        }
                    }

                    rho_2 = 1 - rho_2;
                }

                double random = dis(gen);
                if (random <= rho_2) {
                    next_board.hash_value ^= problem->get_cell_burning_random_bitstring(cell_index, true);
                    next_board.hash_value ^= problem->get_cell_burning_random_bitstring(cell_index, false);
                    next_board.is_each_cell_burning[cell_index] = false;
                    next_board.burning_cell_count--;
                }
            }
            else {
                double rho_1 = 0;

                if (get_cell_fuel_amount(cell_index) > 0) {
                    rho_1 = 1;

                    for (int other_cell_index = 0; other_cell_index < problem->get_cell_count(); ++other_cell_index) {
                        rho_1 *= (1 - problem->get_probability_that_y_ignites_x(cell_index, other_cell_index) * is_cell_burning(other_cell_index));
                    }

                    rho_1 = 1 - rho_1;
                }

                double random = dis(gen);
                if (random <= rho_1) {
                    next_board.hash_value ^= problem->get_cell_burning_random_bitstring(cell_index, false);
                    next_board.hash_value ^= problem->get_cell_burning_random_bitstring(cell_index, true);
                    next_board.is_each_cell_burning[cell_index] = true;
                    next_board.reward += problem->get_cell_burning_reward(cell_index);
                    next_board.burning_cell_count++;
                }
            }
        }
    }

    return next_board;
}

bool TWD::Board::is_final() const {
    return get_burning_cell_count() == 0;
}

int TWD::Board::get_burning_cell_count() const {
    return burning_cell_count;
}
