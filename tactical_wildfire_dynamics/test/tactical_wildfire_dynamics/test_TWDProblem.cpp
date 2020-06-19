#include <catch2/catch.hpp>

#include <tactical_wildfire_dynamics/TWDProblem.h>

SCENARIO("Tactical Widlfire Dynamics Problem") {
    GIVEN("A default 3x3 problem") {
        int grid_width = 3;
        int grid_height = 3;

        TWDProblem problem(grid_width, grid_height);

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
        }
    }
}
