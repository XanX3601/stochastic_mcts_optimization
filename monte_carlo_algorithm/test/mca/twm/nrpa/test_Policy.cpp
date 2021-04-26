#include <mca/twm/nrpa/Policy.h>
#include <mca/twm/nrpa/Sequence.h>

#include <catch2/catch.hpp>
#include <unordered_set>

SCENARIO("monte_carlo_algorithm twm nrpa Policy") {
    GIVEN("A new policy") {
        mca::twm::nrpa::Policy policy;

        WHEN("Getting the weight of any code") {
            THEN("The weight should be 0 by default") {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distribution;

                for (int i = 0; i < 50; ++i) {
                    REQUIRE(policy.get_weight(distribution(gen)) == Approx(0));
                }
            }
        }

        WHEN("Setting the weight of different code") {
            policy.set_weight(0, 3.6);
            policy.set_weight(78, 5.6);
            policy.set_weight(678, -0.4);
            policy.set_weight(888, 10);
            policy.set_weight(6, -7.2);

            THEN("The weight should be retrievable") {
                REQUIRE(policy.get_weight(78) == Approx(5.6));
                REQUIRE(policy.get_weight(888) == Approx(10));
                REQUIRE(policy.get_weight(6) == Approx(-7.2));
                REQUIRE(policy.get_weight(0) == Approx(3.6));
                REQUIRE(policy.get_weight(678) == Approx(-0.4));
            }
        }

        WHEN("Adapting the policy to a sequence") {
            mca::twm::nrpa::Sequence sequence;
            sequence.add_move(0, {0, 1, 2, 3, 4});

            policy.adapt(sequence, 1);

            THEN("The weights should correspond to an expected results") {
                REQUIRE(policy.get_weight(0) == Approx(.8));
                REQUIRE(policy.get_weight(1) == Approx(-.2));
                REQUIRE(policy.get_weight(2) == Approx(-.2));
                REQUIRE(policy.get_weight(3) == Approx(-.2));
                REQUIRE(policy.get_weight(4) == Approx(-.2));
            }
        }
    }
}
