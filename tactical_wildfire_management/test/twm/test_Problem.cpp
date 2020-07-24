#include <catch2/catch.hpp>
#include <twm/Problem.h>
#include <vector>
#include <algorithm>

SCENARIO("tactical_wildfire_management Problem") {
    GIVEN("A default 3x3 problem") {
        int grid_width = 3;
        int grid_height = 3;

        twm::Problem problem(grid_width, grid_height);

        THEN("The index of cell (1, 2) is 7") {
            REQUIRE(problem.cell_to_index({1, 2}) == 7);
        }

        THEN("All cells must have 0 fuel, not be burning, cannot be extinguish and cannot be ignite by any cells") {
            for (int x = 0; x < grid_width; ++x) {
                for (int y = 0; y < grid_height; ++y) {
                    REQUIRE(problem.get_cell_initial_fuel_amount({x, y}) == 0);
                    REQUIRE(problem.is_cell_initially_burning({x, y}) == 0);
                    REQUIRE(problem.get_probability_to_extinguish_cell({x, y}) == Approx(0));

                    for (int nx = 0; nx < grid_width; ++nx) {
                        for (int ny = 0; ny < grid_height; ++ny) {
                            REQUIRE(problem.get_probability_that_y_ignites_x({x, y}, {nx, ny}) == Approx(0));
                        }
                    }
                }
            }

            for (int cell_index = 0; cell_index < grid_width * grid_height; ++cell_index) {
                REQUIRE(problem.get_cell_initial_fuel_amount(cell_index) == 0);
                REQUIRE(problem.is_cell_initially_burning(cell_index) == 0);
                REQUIRE(problem.get_probability_to_extinguish_cell(cell_index) == Approx(0));

                for (int other_cell_index = 0; other_cell_index < grid_width * grid_height; ++other_cell_index) {
                    REQUIRE(problem.get_probability_that_y_ignites_x(cell_index, other_cell_index) == Approx(0));
                }
            }
        }

        THEN("The number of team must be 0") {
            REQUIRE(problem.get_team_count() == 0);
        }

        THEN("All actions must be legal") {
            auto legal_actions = problem.get_legal_actions();

            for (int cell_index = 0; cell_index < problem.get_cell_count(); ++cell_index) {
                REQUIRE(std::find(legal_actions.begin(), legal_actions.end(), twm::Action(problem.index_to_cell(cell_index))) != legal_actions.end());
            }

            REQUIRE(std::find(legal_actions.begin(), legal_actions.end(), twm::Action::null_action) != legal_actions.end());
        }
    }

    GIVEN("A default 3x4 problem") {
        int grid_width = 3;
        int grid_height = 4;

        twm::Problem problem(grid_width, grid_height);

        THEN("The grid width must be 3") {
            REQUIRE(problem.get_grid_width() == 3);
        }

        THEN("The grid height must be 4") {
            REQUIRE(problem.get_grid_height() == 4);
        }

        THEN("The count of cell is 12") {
            REQUIRE(problem.get_cell_count() == 12);
        }

        THEN("The index of cell (0, 0) is 0") {
            REQUIRE(problem.cell_to_index({0, 0}) == 0);
        }

        THEN("The index of cell (2, 3) is 11") {
            REQUIRE(problem.cell_to_index({2, 3}) == 11);
        }

        THEN("The index of cell (1, 3) is 10") {
            REQUIRE(problem.cell_to_index({1, 3}) == 10);
        }
    }

    GIVEN("A problem created with neighborhood ignition probability") {
        int grid_width = 3;
        int grid_height = 4;
        std::vector<int> each_cell_initial_fuel_amount = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        std::vector<bool> is_each_cell_initially_burning(grid_width * grid_height, false);
        double neighbors_ignition_probability = .5;
        std::vector<double> each_cell_extinguishment_probability(grid_width * grid_height, .8);
        std::vector<int> each_cell_burning_reward = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        int team_count = 4;

        twm::Problem problem(grid_width, grid_height, each_cell_initial_fuel_amount, is_each_cell_initially_burning, neighbors_ignition_probability, each_cell_extinguishment_probability, each_cell_burning_reward, team_count);

        THEN("The probabilities of ignition must be 0.5 between neighbors, 1 for the same cell and 0 otherwise") {
            for (int x = 0; x < grid_width; ++x) {
                for (int y = 0; y < grid_height; ++y) {
                    twm::Cell cell(x, y);

                    for (int other_x = 0; other_x < grid_width; ++other_x) {
                        for (int other_y = 0; other_y < grid_height; ++other_y) {
                            twm::Cell other_cell(other_x, other_y);

                            double required_probability = 0;

                            if (cell == other_cell) {
                                required_probability = 1;
                            }
                            else if (cell.is_neighbor(other_cell)) {
                                required_probability = .5;
                            }

                            REQUIRE(problem.get_probability_that_y_ignites_x(cell, other_cell) == Approx(required_probability));
                        }
                    }
                }
            }
        }
    }
}
