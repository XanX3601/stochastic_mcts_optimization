#include <twm/Action.h>
#include <twm/Board.h>

#include <catch2/catch.hpp>
#include <unordered_map>
#include <vector>

SCENARIO("tactical_wildfire_management Board") {
    WHEN("Considering a simple problem and its default board") {
        int grid_width = 3;
        int grid_height = 4;

        std::vector<int> each_cell_initial_fuel_amount = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        std::vector<bool> is_each_cell_initially_burning = {
            false, false, false, false, true, false, false, false, false, false, false, false};
        double neighbors_ignition_probability = .06;
        std::vector<double> each_cell_extinguishment_probability(grid_width * grid_height, .8);
        std::vector<int> each_cell_burning_reward = {0,  -1, -2, -3, -4,  -5,
                                                     -6, -7, -8, -9, -10, -11};
        int team_count = 4;

        twm::Problem problem(grid_width, grid_height, each_cell_initial_fuel_amount,
                             is_each_cell_initially_burning, neighbors_ignition_probability,
                             each_cell_extinguishment_probability, each_cell_burning_reward,
                             team_count);

        twm::Board board(&problem);

        THEN("The default board should be as expected") {
            for (int cell_index = 0; cell_index < problem.get_cell_count(); ++cell_index) {
                REQUIRE(board.get_cell_fuel_amount(cell_index) == cell_index);

                if (cell_index == 4) {
                    REQUIRE(board.is_cell_burning(cell_index));
                } else {
                    REQUIRE_FALSE(board.is_cell_burning(cell_index));
                }
            }

            for (int team_index = 0; team_index < problem.get_team_count(); ++team_index) {
                REQUIRE(board.get_team_cell(team_index) == twm::Cell::null_cell);
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

        twm::Problem problem(grid_width, grid_height, each_cell_initial_fuel_amount,
                             is_each_cell_initially_burning, neighbors_ignition_probability,
                             each_cell_extinguishment_probability, each_cell_burning_reward,
                             team_count);

        twm::Board board(&problem);

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
            {0, 0}, {1, -2}, {2, -10}, {3, -24}, {4, -42},
        };

        std::unordered_map<int, bool> expected_final = {
            {0, false}, {1, false}, {2, false}, {3, false}, {4, false},
        };

        std::unordered_map<int, int> expected_burning_cell_count = {
            {0, 1}, {1, 3}, {2, 6}, {3, 8}, {4, 9},
        };

        std::unordered_map<int, unsigned long long> expected_hash_value;
        unsigned long long hash_value = 0;

        for (int cell_index = 0; cell_index < problem.get_cell_count(); ++cell_index) {
            hash_value ^= problem.get_cell_fuel_random_bitstring(
                cell_index, problem.get_cell_initial_fuel_amount(cell_index));
            hash_value ^= problem.get_cell_burning_random_bitstring(
                cell_index, problem.is_cell_initially_burning(cell_index));
        }

        for (int team_index = 0; team_index < problem.get_team_count(); ++team_index) {
            hash_value ^= problem.get_cell_team_random_bitstring(
                problem.cell_to_index(twm::Cell::null_cell), team_index);
        }

        hash_value ^= problem.get_team_turn_random_bitstring(1);

        expected_hash_value[0] = hash_value;

        hash_value ^= problem.get_cell_fuel_random_bitstring(0, 100);
        hash_value ^= problem.get_cell_fuel_random_bitstring(0, 99);
        hash_value ^= problem.get_cell_burning_random_bitstring(1, false);
        hash_value ^= problem.get_cell_burning_random_bitstring(1, true);
        hash_value ^= problem.get_cell_burning_random_bitstring(3, false);
        hash_value ^= problem.get_cell_burning_random_bitstring(3, true);

        expected_hash_value[1] = hash_value;

        hash_value ^= problem.get_cell_fuel_random_bitstring(0, 99);
        hash_value ^= problem.get_cell_fuel_random_bitstring(0, 98);
        hash_value ^= problem.get_cell_fuel_random_bitstring(1, 100);
        hash_value ^= problem.get_cell_fuel_random_bitstring(1, 99);
        hash_value ^= problem.get_cell_fuel_random_bitstring(3, 100);
        hash_value ^= problem.get_cell_fuel_random_bitstring(3, 99);
        hash_value ^= problem.get_cell_burning_random_bitstring(2, false);
        hash_value ^= problem.get_cell_burning_random_bitstring(2, true);
        hash_value ^= problem.get_cell_burning_random_bitstring(4, false);
        hash_value ^= problem.get_cell_burning_random_bitstring(4, true);
        hash_value ^= problem.get_cell_burning_random_bitstring(6, false);
        hash_value ^= problem.get_cell_burning_random_bitstring(6, true);

        expected_hash_value[2] = hash_value;

        hash_value ^= problem.get_cell_fuel_random_bitstring(0, 98);
        hash_value ^= problem.get_cell_fuel_random_bitstring(0, 97);
        hash_value ^= problem.get_cell_fuel_random_bitstring(1, 99);
        hash_value ^= problem.get_cell_fuel_random_bitstring(1, 98);
        hash_value ^= problem.get_cell_fuel_random_bitstring(2, 100);
        hash_value ^= problem.get_cell_fuel_random_bitstring(2, 99);
        hash_value ^= problem.get_cell_fuel_random_bitstring(3, 99);
        hash_value ^= problem.get_cell_fuel_random_bitstring(3, 98);
        hash_value ^= problem.get_cell_fuel_random_bitstring(4, 100);
        hash_value ^= problem.get_cell_fuel_random_bitstring(4, 99);
        hash_value ^= problem.get_cell_fuel_random_bitstring(6, 100);
        hash_value ^= problem.get_cell_fuel_random_bitstring(6, 99);
        hash_value ^= problem.get_cell_burning_random_bitstring(5, false);
        hash_value ^= problem.get_cell_burning_random_bitstring(5, true);
        hash_value ^= problem.get_cell_burning_random_bitstring(7, false);
        hash_value ^= problem.get_cell_burning_random_bitstring(7, true);

        expected_hash_value[3] = hash_value;

        hash_value ^= problem.get_cell_fuel_random_bitstring(0, 97);
        hash_value ^= problem.get_cell_fuel_random_bitstring(0, 96);
        hash_value ^= problem.get_cell_fuel_random_bitstring(1, 98);
        hash_value ^= problem.get_cell_fuel_random_bitstring(1, 97);
        hash_value ^= problem.get_cell_fuel_random_bitstring(2, 99);
        hash_value ^= problem.get_cell_fuel_random_bitstring(2, 98);
        hash_value ^= problem.get_cell_fuel_random_bitstring(3, 98);
        hash_value ^= problem.get_cell_fuel_random_bitstring(3, 97);
        hash_value ^= problem.get_cell_fuel_random_bitstring(4, 99);
        hash_value ^= problem.get_cell_fuel_random_bitstring(4, 98);
        hash_value ^= problem.get_cell_fuel_random_bitstring(5, 100);
        hash_value ^= problem.get_cell_fuel_random_bitstring(5, 99);
        hash_value ^= problem.get_cell_fuel_random_bitstring(6, 99);
        hash_value ^= problem.get_cell_fuel_random_bitstring(6, 98);
        hash_value ^= problem.get_cell_fuel_random_bitstring(7, 100);
        hash_value ^= problem.get_cell_fuel_random_bitstring(7, 99);
        hash_value ^= problem.get_cell_burning_random_bitstring(8, false);
        hash_value ^= problem.get_cell_burning_random_bitstring(8, true);
        hash_value ^= problem.get_team_turn_random_bitstring(1);
        hash_value ^= problem.get_team_turn_random_bitstring(0);

        expected_hash_value[4] = hash_value;

        int turn_count = 0;

        THEN("When playing null action, the grid should be entirely burning in 5 turn") {
            while (turn_count < 4) {
                board = board.get_next_board(twm::Action::null_action);

                for (int cell_index = 0; cell_index < problem.get_cell_count(); ++cell_index) {
                    REQUIRE(board.get_cell_fuel_amount(cell_index) ==
                            expected_fuel_amount[turn_count][cell_index]);
                    REQUIRE(board.is_cell_burning(cell_index) ==
                            expected_burning_cell[turn_count][cell_index]);
                }
                REQUIRE(board.get_reward() == expected_reward[turn_count]);
                REQUIRE(board.is_final() == expected_final[turn_count]);
                REQUIRE(board.get_burning_cell_count() == expected_burning_cell_count[turn_count]);
                REQUIRE(board.get_hash_value() == expected_hash_value[turn_count]);

                board = board.get_next_board(twm::Action::null_action);
                turn_count++;
            }
            for (int cell_index = 0; cell_index < problem.get_cell_count(); ++cell_index) {
                REQUIRE(board.get_cell_fuel_amount(cell_index) ==
                        expected_fuel_amount[turn_count][cell_index]);
                REQUIRE(board.is_cell_burning(cell_index) ==
                        expected_burning_cell[turn_count][cell_index]);
            }
            REQUIRE(board.get_reward() == expected_reward[turn_count]);
            REQUIRE(board.is_final() == expected_final[turn_count]);
            REQUIRE(board.get_burning_cell_count() == expected_burning_cell_count[turn_count]);
            REQUIRE(board.get_hash_value() == expected_hash_value[turn_count]);
        }

        THEN(
            "When playing null action at first and second turn then reseting the reward, it should "
            "equal -8") {
            board = board.get_next_board(twm::Action::null_action)
                        .get_next_board(twm::Action::null_action)
                        .get_next_board(twm::Action::null_action)
                        .get_next_board(twm::Action::null_action);

            board.reset_reward();

            REQUIRE(board.get_reward() == -8);
        }

        THEN("When scaling fuel amount by 0.5, it should be half the initial amount") {
            board.scale_fuel_amount(.5);

            for (int cell_index = 0; cell_index < problem.get_cell_count(); ++cell_index) {
                REQUIRE(board.get_cell_fuel_amount(cell_index) ==
                        expected_fuel_amount[0][cell_index] / 2);
            }
        }
    }

    WHEN("Considering a simple 3x3 problem") {
        int grid_width = 3;
        int grid_height = 3;
        std::vector<int> each_cell_initial_fuel_amount = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::vector<bool> is_each_cell_initially_burning(9, false);
        is_each_cell_initially_burning[0] = true;
        is_each_cell_initially_burning[1] = true;
        is_each_cell_initially_burning[2] = true;
        double neighbors_ignition_probability = .5;
        std::vector<double> each_cell_ignition_probabilities(9, .5);
        std::vector<int> each_cell_burning_reward = {-1, -2, -3, -4, -5, -6, -7, -8, -9};
        int team_count = 2;

        ::twm::Problem problem(grid_width, grid_height, each_cell_initial_fuel_amount,
                               is_each_cell_initially_burning, neighbors_ignition_probability,
                               each_cell_ignition_probabilities, each_cell_burning_reward,
                               team_count);

        ::twm::Board board(&problem);

        THEN("The lowest possible reward is -285") {
            REQUIRE(board.lowest_possible_reward() == -291);
        }

        THEN("The highest possible reward is -1") {
            REQUIRE(board.highest_possible_reward() == -7);
        }
    }
}
