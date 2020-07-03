#include <catch2/catch.hpp>
#include <tactical_wildfire_dynamics/Board.h>
#include <tactical_wildfire_dynamics/Action.h>
#include <vector>
#include <unordered_map>

SCENARIO("Tacticlca Wildfire Dynamics Board") {
    WHEN("Considering a simple problem and its default board") {
        int grid_width = 3;
        int grid_height = 4;

        std::vector<int> each_cell_initial_fuel_amount = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        std::vector<bool> is_each_cell_initially_burning = {false, false, false, false, true, false, false, false, false, false, false, false};
        double neighbors_ignition_probability = .06;
        std::vector<double> each_cell_extinguishment_probability(grid_width * grid_height, .8);
        std::vector<int> each_cell_burning_reward = {0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11};
        int team_count = 4;

        TWD::Problem problem(grid_width, grid_height, each_cell_initial_fuel_amount, is_each_cell_initially_burning, neighbors_ignition_probability, each_cell_extinguishment_probability, each_cell_burning_reward, team_count);

        TWD::Board board(&problem);

        THEN("The default board should be as expected") {
            for (int cell_index = 0; cell_index < problem.get_cell_count(); ++cell_index) {
                REQUIRE(board.get_cell_fuel_amount(cell_index) == cell_index);

                if (cell_index == 4) {
                    REQUIRE(board.is_cell_burning(cell_index));
                }
                else {
                    REQUIRE_FALSE(board.is_cell_burning(cell_index));
                }
            }

            for (int team_index = 0; team_index < problem.get_team_count(); ++team_index) {
                REQUIRE(board.get_team_cell(team_index) == TWD::Cell(0, 0));
            }

            REQUIRE(board.get_next_team_index_to_play() == 0);

            REQUIRE(board.get_reward() == -4);
        }
    }

    WHEN("Considering an impossible problem") {
        int grid_width = 3;
        int grid_height = 3;

        std::vector<int> each_cell_initial_fuel_amount(grid_width * grid_height, 100);
        std::vector<bool> is_each_cell_initially_burning(grid_width * grid_height, false);
        is_each_cell_initially_burning[0] = true;
        double neighbors_ignition_probability = 1;
        std::vector<double> each_cell_extinguishment_probability(grid_width * grid_height, 0);
        std::vector<int> each_cell_burning_reward = {0, -1, -2, -1, -2, -3, -2, -3, -4};
        int team_count = 2;

        TWD::Problem problem(grid_width, grid_height, each_cell_initial_fuel_amount, is_each_cell_initially_burning, neighbors_ignition_probability, each_cell_extinguishment_probability, each_cell_burning_reward, team_count);

        TWD::Board board(&problem);

        std::unordered_map<int, std::vector<int>> expected_fuel_amount = {
            {0, {100, 100, 100, 100, 100, 100, 100, 100, 100}},
            {1, {99, 100, 100, 100, 100, 100, 100, 100, 100}},
            {2, {98, 99, 100, 99, 100, 100, 100, 100, 100}},
            {3, {97, 98, 99, 98, 99, 100, 99, 100, 100}},
            {4, {96, 97, 98, 97, 98, 99, 98, 99, 100}},
        };

        std::unordered_map<int, std::vector<bool>> expected_burning_cell = {
            {0, {true, false, false, false, false, false, false, false, false}},
            {1, {true, true, false, true, false, false, false, false, false}},
            {2, {true, true, true, true, true, false, true, false, false}},
            {3, {true, true, true, true, true, true, true, true, false}},
            {4, {true, true, true, true, true, true, true, true, true}},
        };

        std::unordered_map<int, int> expected_reward = {
            {0, 0},
            {1, -2},
            {2, -10},
            {3, -24},
            {4, -42},
        };

        std::unordered_map<int, bool> expected_final = {
            {0, false},
            {1, false},
            {2, false},
            {3, false},
            {4, true},
        };

        std::unordered_map<int, int> expected_burning_cell_count = {
            {0, 1},
            {1, 3},
            {2, 6},
            {3, 8},
            {4, 9},
        };

        int turn_count = 0;

        THEN("When playing always the same action, the grid should be entirely burning") {
            while(turn_count < 4) {
                board = board.get_next_board({{0, 0}});

                for (int cell_index = 0; cell_index < problem.get_cell_count(); ++cell_index) {
                    REQUIRE(board.get_cell_fuel_amount(cell_index) == expected_fuel_amount[turn_count][cell_index]);
                    REQUIRE(board.is_cell_burning(cell_index) == expected_burning_cell[turn_count][cell_index]);
                }
                REQUIRE(board.get_reward() == expected_reward[turn_count]);
                REQUIRE(board.is_final() == expected_final[turn_count]);
                REQUIRE(board.get_burning_cell_count() == expected_burning_cell_count[turn_count]);

                board = board.get_next_board({{0, 0}});
                turn_count++;
            }
            for (int cell_index = 0; cell_index < problem.get_cell_count(); ++cell_index) {
                REQUIRE(board.get_cell_fuel_amount(cell_index) == expected_fuel_amount[turn_count][cell_index]);
                REQUIRE(board.is_cell_burning(cell_index) == expected_burning_cell[turn_count][cell_index]);
            }
            REQUIRE(board.get_reward() == expected_reward[turn_count]);
            REQUIRE(board.is_final() == expected_final[turn_count]);
            REQUIRE(board.get_burning_cell_count() == expected_burning_cell_count[turn_count]);
        }
    }
}
