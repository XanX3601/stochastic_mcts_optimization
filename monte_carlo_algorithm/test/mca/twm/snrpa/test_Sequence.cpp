#include <mca/twm/snrpa/Sequence.h>

#include <catch2/catch.hpp>

SCENARIO("monte_carlo_algorithm twm snrpa Sequence") {
    GIVEN("A basic sequence 2, 0, 1, 5, 4") {
        std::vector<int> codes = {2, 0, 1, 5, 4};
        mca::twm::snrpa::Sequence sequence(codes);

        THEN("Its size is 5") { REQUIRE(sequence.get_code_count() == 5); }

        THEN("Code 2 is at index 0") { REQUIRE(sequence.get_code(0) == 2); }

        THEN("Code 0 is at index 1") { REQUIRE(sequence.get_code(1) == 0); }

        THEN("Code 1 is at index 2") { REQUIRE(sequence.get_code(2) == 1); }

        THEN("Code 5 is at index 3") { REQUIRE(sequence.get_code(3) == 5); }

        THEN("Code 4 is at index 4") { REQUIRE(sequence.get_code(4) == 4); }
    }

    GIVEN("Any sequence") {
        mca::twm::snrpa::Sequence sequence({});

        THEN("Its mean score is nan by default") { REQUIRE(std::isnan(sequence.get_mean_score())); }

        WHEN("Closing the sequence") {
            sequence.close();

            THEN("When adding a score, it must throw an error") {
                REQUIRE_THROWS(sequence.add_score(5));
            }
        }

        WHEN("Adding score -5, -2 and -42 to the sequence") {
            sequence.add_score(-5);
            sequence.add_score(-2);
            sequence.add_score(-42);

            THEN("Its mean score should be 16,3333") {
                REQUIRE(sequence.get_mean_score() == Approx(-16.3333333333));
            }
        }
    }
}
