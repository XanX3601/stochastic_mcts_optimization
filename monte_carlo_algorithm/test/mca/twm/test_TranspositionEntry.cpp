#include <mca/twm/TranspositionEntry.h>

#include <catch2/catch.hpp>

SCENARIO("monte_carlo_algorithm twm TranspositionEntry") {
    GIVEN("A typical TranspositionEntry") {
        mca::twm::TranspositionEntry entry(0);

        THEN("By default, its accumulated reward and number of playout is 0") {
            REQUIRE(entry.get_hash_value() == 0);
            REQUIRE(entry.get_number_of_playout() == 0);
            REQUIRE(entry.get_accumulated_reward() == 0);
        }

        THEN("After some update, the entry fields must be consistent") {
            entry.update({{0, 0}}, -5, 0);
            entry.update({{1, 0}}, -10, 0);
            entry.update({{1, 1}}, -15, 1);
            entry.update({{0, 0}}, -20, 1);

            REQUIRE(entry.does_contain({{0, 0}}));
            REQUIRE(entry.does_contain({{1, 0}}));
            REQUIRE(entry.does_contain({{1, 1}}));
            REQUIRE_FALSE(entry.does_contain({{1, 2}}));

            REQUIRE(entry.get_accumulated_reward() == -50);
            REQUIRE(entry.get_action_accumulated_reward({{0, 0}}) == -25);
            REQUIRE(entry.get_action_accumulated_reward({{1, 0}}) == -10);
            REQUIRE(entry.get_action_accumulated_reward({{1, 1}}) == -15);

            REQUIRE(entry.get_number_of_playout() == 4);
            REQUIRE(entry.get_action_number_of_playout({{0, 0}}) == 2);
            REQUIRE(entry.get_action_number_of_playout({{1, 0}}) == 1);
            REQUIRE(entry.get_action_number_of_playout({{1, 1}}) == 1);

            REQUIRE(entry.get_action_number_of_unique_hash_value({{0, 0}}) == 2);
            REQUIRE(entry.get_action_number_of_unique_hash_value({{1, 0}}) == 1);
            REQUIRE(entry.get_action_number_of_unique_hash_value({{1, 1}}) == 1);
        }
    }
}
