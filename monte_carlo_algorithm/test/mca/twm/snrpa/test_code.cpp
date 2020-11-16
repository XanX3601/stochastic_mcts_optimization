#include <mca/twm/snrpa/code.h>
#include <twm/Board.h>
#include <twm/Problem.h>

#include <catch2/catch.hpp>
#include <unordered_set>
#include <vector>

SCENARIO("monte_carlo_algorithm twm snrpa code") {
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

        WHEN("Computing all codes") {
            std::unordered_set<int> codes = mca::twm::snrpa::compute_all_codes(problem);

            THEN("All codes should be computed") {
                for (int code = -1; code < 200; ++code) {
                    REQUIRE(codes.find(code) != codes.end());
                }
            }

            THEN("There should be 201 codes") { REQUIRE(codes.size() == 201); }
        }

        WHEN("Codify and decodify action") {
            twm::Board board(&problem);

            THEN("All codes should be what they are expected") {
                REQUIRE(mca::twm::snrpa::codify_action(board, {{0, 0}}) == 0);
                REQUIRE(mca::twm::snrpa::codify_action(board, {{1, 0}}) == 1);
                REQUIRE(mca::twm::snrpa::codify_action(board, {{0, 4}}) == 120);

                REQUIRE(mca::twm::snrpa::decodify_action(problem, 0) == twm::Action({0, 0}));
                REQUIRE(mca::twm::snrpa::decodify_action(problem, 49) == twm::Action({4, 4}));
                REQUIRE(mca::twm::snrpa::decodify_action(problem, 120) == twm::Action({0, 4}));
            }
        }

        THEN("Ammong all codes, any greater to 100 targets a burning cell") {
            for (int code : mca::twm::snrpa::compute_all_codes(problem)) {
                if (code >= 100) {
                    REQUIRE(mca::twm::snrpa::does_code_target_burning_cell(problem, code));
                } else {
                    REQUIRE_FALSE(mca::twm::snrpa::does_code_target_burning_cell(problem, code));
                }
            }
        }

        THEN("Code -1 target no team (team index -1)") {
            REQUIRE(mca::twm::snrpa::get_target_team_index(problem, -1) == -1);
        }

        THEN("Code 0 to 24 target team 0") {
            for (int code = 0; code < 25; ++code) {
                REQUIRE(mca::twm::snrpa::get_target_team_index(problem, code) == 0);
            }
        }

        THEN("Code 100 to 124 target team 0") {
            for (int code = 100; code < 124; ++code) {
                REQUIRE(mca::twm::snrpa::get_target_team_index(problem, code) == 0);
            }
        }

        THEN("Code 25 to 49 target team 1") {
            for (int code = 25; code < 50; ++code) {
                REQUIRE(mca::twm::snrpa::get_target_team_index(problem, code) == 1);
            }
        }

        THEN("Code 125 to 149 target team 1") {
            for (int code = 125; code < 150; ++code) {
                REQUIRE(mca::twm::snrpa::get_target_team_index(problem, code) == 1);
            }
        }

        THEN("Code 50 to 74 target team 2") {
            for (int code = 50; code < 74; ++code) {
                REQUIRE(mca::twm::snrpa::get_target_team_index(problem, code) == 2);
            }
        }

        THEN("Code 150 to 174 target team 2") {
            for (int code = 150; code < 174; ++code) {
                REQUIRE(mca::twm::snrpa::get_target_team_index(problem, code) == 2);
            }
        }

        THEN("Code 75 to 99 target team 3") {
            for (int code = 75; code < 99; ++code) {
                REQUIRE(mca::twm::snrpa::get_target_team_index(problem, code) == 3);
            }
        }

        THEN("Code 175 to 199 target team 3") {
            for (int code = 175; code < 199; ++code) {
                REQUIRE(mca::twm::snrpa::get_target_team_index(problem, code) == 3);
            }
        }

        WHEN("Considering the root board of the problem") {
            twm::Board board(&problem);

            THEN("Codes from -1 to 19 are legal") {
                for (int code = -1; code < 19; ++code) {
                    REQUIRE(mca::twm::snrpa::is_code_legal(board, code));
                }
            }

            THEN("Code 20 is illegal") { REQUIRE_FALSE(mca::twm::snrpa::is_code_legal(board, 20)); }

            THEN("Codes 21 to 24 are legal") {
                for (int code = 21; code < 25; ++code) {
                    REQUIRE(mca::twm::snrpa::is_code_legal(board, code));
                }
            }

            THEN("Codes 25 to 119 are illegal") {
                for (int code = 25; code < 120; ++code) {
                    REQUIRE_FALSE(mca::twm::snrpa::is_code_legal(board, code));
                }
            }

            THEN("Code 120 is legal") { REQUIRE(mca::twm::snrpa::is_code_legal(board, 120)); }

            THEN("Codes 121 to 199 are illegal") {
                for (int code = 121; code < 200; ++code) {
                    REQUIRE_FALSE(mca::twm::snrpa::is_code_legal(board, code));
                }
            }
        }

        THEN("The index of code -1 is 0") { REQUIRE(mca::twm::snrpa::code_to_index(-1) == 0); }

        THEN("The code corresponding to index 0 is -1") {
            REQUIRE(mca::twm::snrpa::index_to_code(0) == -1);
        }
    }
}
