#include <mca/twm/nrpa/Sequence.h>

#include <catch2/catch.hpp>

SCENARIO("monte_carlo_algorithm twm nrpa Sequence") {
    GIVEN("An empty sequence") {
        mca::twm::nrpa::Sequence sequence;

        WHEN("Adding 5 elements to this sequence") {
            sequence.add_move(0, {0, 1, 2});
            sequence.add_move(15, {1, 14});
            sequence.add_move(67, {67, 43, 5});
            sequence.add_move(9, {5});
            sequence.add_move(78, {69, 89});

            THEN("The move count should equal 5") { REQUIRE(sequence.get_move_count() == 5); }

            THEN("Each move should be retrievable") {
                REQUIRE(sequence.get_chosen_code(0) == 0);
                REQUIRE(sequence.get_chosen_code(4) == 78);
                REQUIRE(sequence.get_chosen_code(1) == 15);
                REQUIRE(sequence.get_chosen_code(2) == 67);
                REQUIRE(sequence.get_chosen_code(3) == 9);

                REQUIRE(sequence.get_possible_codes(0) == std::vector<int>({0, 1, 2}));
                REQUIRE(sequence.get_possible_codes(4) == std::vector<int>({69, 89}));
                REQUIRE(sequence.get_possible_codes(1) == std::vector<int>({1, 14}));
                REQUIRE(sequence.get_possible_codes(2) == std::vector<int>({67, 43, 5}));
                REQUIRE(sequence.get_possible_codes(3) == std::vector<int>({5}));
            }
        }
    }
}
