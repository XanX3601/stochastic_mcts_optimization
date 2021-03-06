#include <twm/Board.h>

#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>
#include <unordered_set>
#include <vector>

twm::Board::Board(twm::Problem* problem) : problem(problem) {
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

    each_team_cell = std::vector<twm::Cell>(problem->get_team_count(), twm::Cell::null_cell);
    for (int team_index = 0; team_index < problem->get_team_count(); ++team_index) {
        hash_value ^= problem->get_cell_team_random_bitstring(
            problem->cell_to_index(twm::Cell::null_cell), team_index);
    }
}

int twm::Board::get_cell_fuel_amount(const twm::Cell& cell) const {
    return get_cell_fuel_amount(problem->cell_to_index(cell));
}

int twm::Board::get_cell_fuel_amount(int cell_index) const {
    return each_cell_fuel_amount[cell_index];
}

bool twm::Board::is_cell_burning(const twm::Cell& cell) const {
    return is_cell_burning(problem->cell_to_index(cell));
}

bool twm::Board::is_cell_burning(int cell_index) const { return is_each_cell_burning[cell_index]; }

twm::Cell twm::Board::get_team_cell(int team_index) const { return each_team_cell[team_index]; }

int twm::Board::get_next_team_index_to_play() const { return next_team_index_to_play; }

int twm::Board::get_reward() const { return reward; }

bool twm::Board::is_action_legal(const twm::Action& action) const {
    return (0 <= action.cell.x && action.cell.x < problem->get_grid_width() && 0 <= action.cell.y &&
            action.cell.y < problem->get_grid_height()) ||
           action.is_null();
}

twm::Board twm::Board::get_next_board(const twm::Action& action) const {
    if (!(is_action_legal(action))) {
        throw std::invalid_argument("illegal action received");
    }

    twm::Board next_board(*this);

    next_board.hash_value ^= problem->get_cell_team_random_bitstring(
        get_team_cell(next_team_index_to_play), next_team_index_to_play);
    next_board.hash_value ^=
        problem->get_cell_team_random_bitstring(action.cell, next_team_index_to_play);
    next_board.each_team_cell[next_team_index_to_play] = action.cell;
    next_board.hash_value ^= problem->get_team_turn_random_bitstring(next_team_index_to_play);
    next_board.next_team_index_to_play++;

    if (next_board.get_next_team_index_to_play() == problem->get_team_count()) {
        next_board.next_team_index_to_play = 0;
        next_board.hash_value ^= problem->get_team_turn_random_bitstring(0);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0, 1);

        std::unordered_set<int> extinguished_cells_index;

        for (int team_index = 0; team_index < problem->get_team_count(); ++team_index) {
            Cell team_cell = get_team_cell(team_index);
            int team_cell_index = problem->cell_to_index(team_cell);

            if (problem->is_in_grid(team_cell) && is_cell_burning(team_cell_index) &&
                get_cell_fuel_amount(team_cell_index) > 0) {
                double random = dis(gen);

                if (random < problem->get_probability_to_extinguish_cell(team_cell_index)) {
                    if (next_board.is_cell_burning(team_cell_index)) {
                        next_board.hash_value ^=
                            problem->get_cell_burning_random_bitstring(team_cell_index, true);
                        next_board.hash_value ^=
                            problem->get_cell_burning_random_bitstring(team_cell_index, false);
                        next_board.is_each_cell_burning[team_cell_index] = false;
                        next_board.burning_cell_count--;
                        extinguished_cells_index.insert(team_cell_index);
                    }
                }
            }
        }

        for (int cell_index = 0; cell_index < problem->get_cell_count(); ++cell_index) {
            if (is_cell_burning(cell_index)) {
                double rho_2 = 1;

                if (get_cell_fuel_amount(cell_index) > 0) {
                    int cell_fuel_amount = get_cell_fuel_amount(cell_index);
                    next_board.hash_value ^=
                        problem->get_cell_fuel_random_bitstring(cell_index, cell_fuel_amount);
                    next_board.hash_value ^=
                        problem->get_cell_fuel_random_bitstring(cell_index, cell_fuel_amount - 1);
                    next_board.each_cell_fuel_amount[cell_index] = cell_fuel_amount - 1;

                    rho_2 = 0;
                }

                double random = dis(gen);
                if (random < rho_2) {
                    next_board.hash_value ^=
                        problem->get_cell_burning_random_bitstring(cell_index, true);
                    next_board.hash_value ^=
                        problem->get_cell_burning_random_bitstring(cell_index, false);
                    next_board.is_each_cell_burning[cell_index] = false;
                    next_board.burning_cell_count--;
                } else {
                    next_board.reward += problem->get_cell_burning_reward(cell_index);
                }
            } else {
                double rho_1 = 0;

                if (get_cell_fuel_amount(cell_index) > 0) {
                    rho_1 = 1;

                    Cell cell = problem->index_to_cell(cell_index);

                    for (int x_diff = -1; x_diff < 2; ++x_diff) {
                        for (int y_diff = -1; y_diff < 2; ++y_diff) {
                            Cell neighbor{cell.x + x_diff, cell.y + y_diff};
                            int neighbor_index = problem->cell_to_index(neighbor);

                            if (problem->is_in_grid(neighbor) && neighbor != cell &&
                                !extinguished_cells_index.count(neighbor_index)) {
                                rho_1 *= (1 - problem->get_probability_that_y_ignites_x(
                                                  cell_index, neighbor_index) *
                                                  is_cell_burning(neighbor_index));
                            }
                        }
                    }

                    rho_1 = 1 - rho_1;
                }

                double random = dis(gen);
                if (random <= rho_1) {
                    next_board.hash_value ^=
                        problem->get_cell_burning_random_bitstring(cell_index, false);
                    next_board.hash_value ^=
                        problem->get_cell_burning_random_bitstring(cell_index, true);
                    next_board.is_each_cell_burning[cell_index] = true;
                    next_board.reward += problem->get_cell_burning_reward(cell_index);
                    next_board.burning_cell_count++;
                }
            }
        }
    } else {
        next_board.hash_value ^=
            problem->get_team_turn_random_bitstring(next_board.next_team_index_to_play);
    }

    return next_board;
}

bool twm::Board::is_final() const { return get_burning_cell_count() == 0; }

int twm::Board::get_burning_cell_count() const { return burning_cell_count; }

unsigned long long twm::Board::get_hash_value() const { return hash_value; }

const std::vector<twm::Action>& twm::Board::get_legal_actions() const {
    return problem->get_legal_actions();
}

void twm::Board::reset_reward() {
    reward = 0;

    for (int cell_index = 0; cell_index < problem->get_cell_count(); ++cell_index) {
        if (is_cell_burning(cell_index)) {
            reward += problem->get_cell_burning_reward(cell_index);
        }
    }
}

void twm::Board::scale_fuel_amount(double scale) {
    for (int cell_index = 0; cell_index < problem->get_cell_count(); ++cell_index) {
        each_cell_fuel_amount[cell_index] *= scale;
    }
}

int twm::Board::lowest_possible_reward() const {
    int lowest_possible_reward = reward;
    for (int cell_index = 0; cell_index < problem->get_cell_count(); ++cell_index) {
        lowest_possible_reward +=
            get_cell_fuel_amount(cell_index) * problem->get_cell_burning_reward(cell_index);
    }
    return lowest_possible_reward;
}

int twm::Board::highest_possible_reward() const {
    int highest_possible_reward = reward;
    std::vector<int> cell_indexes(problem->get_cell_count());
    std::vector<bool> is_each_cell_burning = this->is_each_cell_burning;
    int burning_cell_count = this->get_burning_cell_count();

    std::iota(cell_indexes.begin(), cell_indexes.end(), 0);

    std::sort(cell_indexes.begin(), cell_indexes.end(), [&](int cell_a, int cell_b) {
        return problem->get_cell_burning_reward(cell_a) < problem->get_cell_burning_reward(cell_b);
    });

    while (burning_cell_count > 0) {
        for (int team_index = 0; team_index < problem->get_team_count(); ++team_index) {
            if (burning_cell_count > 0) {
                bool cell_to_extinguish_found = false;
                int index = 0;

                while (!cell_to_extinguish_found) {
                    if (is_each_cell_burning[cell_indexes[index]]) {
                        is_each_cell_burning[cell_indexes[index]] = false;
                        burning_cell_count--;
                        cell_to_extinguish_found = true;
                    } else {
                        index++;
                    }
                }
            }
        }

        for (auto cell_index : cell_indexes) {
            if (is_each_cell_burning[cell_index]) {
                highest_possible_reward += problem->get_cell_burning_reward(cell_index);
            }
        }
    }

    return highest_possible_reward;
}

twm::Problem const& twm::Board::get_problem() const { return *problem; }
