#include <mca/twm/nrpa/NRPA.h>

#include <catch2/catch.hpp>

SCENARIO("monte_carlo_algorithm twm nrpa NRPA") {
    GIVEN("A twm problem to solve using NRPA") {
        int grid_width = 5;
        int grid_height = 5;
        int cell_amount = grid_width * grid_height;
        std::vector<int> each_cell_initial_fuel_amount(cell_amount, 10);
        std::vector<bool> is_each_cell_initialy_burning(cell_amount, false);
        is_each_cell_initialy_burning[0] = true;
        double neighbors_ignition_probability = 0.06;
        std::vector<double> each_cell_extinguishment_probability(cell_amount, .8);
        std::vector<int> each_cell_burning_reward(cell_amount, -1);
        int team_count = 4;

        ::twm::Problem problem(grid_width, grid_height, each_cell_initial_fuel_amount,
                               is_each_cell_initialy_burning, neighbors_ignition_probability,
                               each_cell_extinguishment_probability, each_cell_burning_reward,
                               team_count);

        ::twm::Board root_board(&problem);

        mca::twm::nrpa::NRPA nrpa(root_board, 2, 20, .5);

        nrpa.search();
    }
}
