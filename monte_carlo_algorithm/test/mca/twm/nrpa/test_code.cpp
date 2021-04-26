#include <mca/twm/nrpa/code.h>
#include <twm/Board.h>
#include <twm/Problem.h>

#include <catch2/catch.hpp>
#include <unordered_set>
#include <vector>

SCENARIO("monte_carlo_algorithm twm nrpa code") {
    GIVEN("A basic problem") {
        int grid_width = 5;
        int grid_height = 5;
        int cell_amount = grid_width * grid_height;
        std::vector<int> each_cell_initial_fuel_amount(cell_amount, 10);
        std::vector<bool> is_each_cell_initialy_burning(cell_amount, false);
        is_each_cell_initialy_burning[20] = true;
        double neighbors_ignition_probability = 0.06;
        std::vector<double> each_cell_extinguishment_probability(cell_amount, .8);
        std::vector<int> each_cell_burning_reward(cell_amount, -1);
        int team_count = 4;

        ::twm::Problem problem(grid_width, grid_height, each_cell_initial_fuel_amount,
                               is_each_cell_initialy_burning, neighbors_ignition_probability,
                               each_cell_extinguishment_probability, each_cell_burning_reward,
                               team_count);

        WHEN("Codify and decodify action") {
            twm::Board board(&problem);

            THEN("All codes should be what they are expected") {
                REQUIRE(mca::twm::nrpa::codify_action(board, {{0, 0}}) == 0);
                REQUIRE(mca::twm::nrpa::codify_action(board, {{1, 0}}) == 1);
                REQUIRE(mca::twm::nrpa::codify_action(board, {{0, 4}}) == 120);

                REQUIRE(mca::twm::nrpa::decodify_action(problem, 0) == twm::Action({0, 0}));
                REQUIRE(mca::twm::nrpa::decodify_action(problem, 49) == twm::Action({4, 4}));
                REQUIRE(mca::twm::nrpa::decodify_action(problem, 120) == twm::Action({0, 4}));
            }
        }
    }
}
