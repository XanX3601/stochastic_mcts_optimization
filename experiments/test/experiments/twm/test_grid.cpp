#include <experiments/twm/grid.h>
#include <twm/Cell.h>
#include <twm/Problem.h>

#include <catch2/catch.hpp>
#include <cmath>

SCENARIO("experiments grid") {
    WHEN("Generating problem 1") {
        twm::Problem problem = generate_problem_1(8, 4);

        THEN("The resulting problem must correspond to what is expected") {
            REQUIRE(problem.get_grid_width() == 8);
            REQUIRE(problem.get_grid_height() == 8);
            REQUIRE(problem.get_cell_count() == 8 * 8);
            REQUIRE(problem.get_team_count() == 4);

            twm::Cell bottom_left_corner(0, 7);
            twm::Cell top_right_corner(7, 0);

            for (int cell_index = 0; cell_index < problem.get_cell_count();
                 ++cell_index) {
                twm::Cell cell = problem.index_to_cell(cell_index);

                REQUIRE(problem.get_cell_initial_fuel_amount(cell_index) == 66);

                if (problem.index_to_cell(cell_index) == bottom_left_corner) {
                    REQUIRE(problem.is_cell_initially_burning(cell_index));
                } else {
                    REQUIRE_FALSE(
                        problem.is_cell_initially_burning(cell_index));
                }

                REQUIRE(problem.get_probability_to_extinguish_cell(
                            cell_index) == Approx(.8));

                if (cell == top_right_corner) {
                    REQUIRE(problem.get_cell_burning_reward(cell_index) == -10);
                } else {
                    int manhattan_distance_to_bottom_left_corner =
                        std::abs(cell.x - bottom_left_corner.x) +
                        std::abs(cell.y - bottom_left_corner.y);
                    REQUIRE(problem.get_cell_burning_reward(cell_index) ==
                            -1 * manhattan_distance_to_bottom_left_corner - 1);
                }

                for (int other_cell_index = 0;
                     other_cell_index < problem.get_cell_count();
                     ++other_cell_index) {
                    if (cell_index == other_cell_index) {
                        REQUIRE(problem.get_probability_that_y_ignites_x(
                                    cell_index, other_cell_index) == Approx(1));
                    } else if (cell.is_neighbor(
                                   problem.index_to_cell(other_cell_index))) {
                        REQUIRE(problem.get_probability_that_y_ignites_x(
                                    cell_index, other_cell_index) ==
                                Approx(.06));
                    } else {
                        REQUIRE(problem.get_probability_that_y_ignites_x(
                                    cell_index, other_cell_index) == 0);
                    }
                }
            }
        }

        THEN("Grid 1 can be generated from the problem") {
            twm::Board board = generate_grid_1(&problem);

            twm::Cell bottom_left_corner(0, problem.get_grid_height() - 1);

            REQUIRE(board.get_cell_fuel_amount(bottom_left_corner) == 0);
            REQUIRE(board.is_cell_burning(bottom_left_corner));
            REQUIRE(board.get_next_team_index_to_play() == 0);
            REQUIRE_FALSE(board.is_final());
        }
    }
}
