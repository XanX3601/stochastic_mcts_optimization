#include <catch2/catch.hpp>
#include <mca/twm/TranspositionTable.h>

SCENARIO("monte_carlo_algorithm twm TranspositionTable") {
    GIVEN("An empty TranspositionTable") {
        mca::twm::TranspositionTable table;

        THEN("It does not contain the entry 0") {
            REQUIRE_FALSE(table.does_contain(0));
        }

        WHEN("Inserting entries 0, 1 and 2") {
            table.insert(0);
            table.insert(1);
            table.insert(2);

            THEN("The table must contain 3 entries") {
                REQUIRE(table.get_number_of_entries() == 3);
            }

            THEN("Any updates on any entry should be saved") {
                table.get_entry(0).update({{0, 0}}, -10);
                table.get_entry(0).update({{0, 0}}, -5);

                REQUIRE(table.get_entry(0).get_hash_value() == 0);
                REQUIRE(table.get_entry(0).get_accumulated_reward() == -15);
                REQUIRE(table.get_entry(0).get_number_of_playout() == 2);
            }
        }
    }
}
