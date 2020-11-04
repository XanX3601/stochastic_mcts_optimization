#include <mca/twm/snrpa/Policy.h>
#include <mca/twm/snrpa/Sequence.h>

#include <catch2/catch.hpp>
#include <unordered_set>

SCENARIO("monte_carlo_algorithm twm snrpa Policy") {
    GIVEN("A policy of 6 codes from 0 to 5") {
        std::unordered_set<int> codes;

        for (int code = 0; code < 6; ++code) {
            codes.insert(code);
        }

        mca::twm::snrpa::Policy policy(codes);

        WHEN("Adapting the policy to sequence 3, 5, 0, 4, 1, 2") {
            mca::twm::snrpa::Sequence sequence({3, 5, 0, 4, 1, 2});

            policy.adapt(sequence, .5);

            THEN("3 weight should be 0.4166666667") {
                REQUIRE(policy.get_code_weight(3) == Approx(.4166666667));
            }

            THEN("5 weight should be 0.316666666666667") {
                REQUIRE(policy.get_code_weight(5) == Approx(.316666666666667));
            }

            THEN("0 weight should be 0.191666666666667") {
                REQUIRE(policy.get_code_weight(0) == Approx(.191666666666667));
            }

            THEN("4 weight should be 0.025") { REQUIRE(policy.get_code_weight(4) == Approx(.025)); }

            THEN("1 weight should be -0.225") {
                REQUIRE(policy.get_code_weight(1) == Approx(-.225));
            }

            THEN("2 weight should be -.725") {
                REQUIRE(policy.get_code_weight(2) == Approx(-.725));
            }
        }

        WHEN("Generating a sequence") {
            mca::twm::snrpa::Sequence sequence = policy.generate_sequence();

            THEN("It must contains all code codes") {
                REQUIRE(sequence.get_code_count() == 6);

                std::unordered_set<int> codes_in_sequence;
                for (int code_index = 0; code_index < sequence.get_code_count(); ++code_index) {
                    codes_in_sequence.insert(sequence.get_code(code_index));
                }

                for (int code = 0; code < 6; ++code) {
                    REQUIRE(codes_in_sequence.find(code) != codes_in_sequence.end());
                }
            }
        }
    }
}
