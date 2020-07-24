#include <twm/Cell.h>
#include <catch2/catch.hpp>

SCENARIO("tactical_wildfire_management Cell") {
    WHEN("Considering cell (4, 5)") {
        twm::Cell cell(4, 5);

        THEN("Cell (3, 5), (5, 5), (4, 4) and (4, 6) are its only neighbors") {
            REQUIRE(cell.is_neighbor({3, 5}));
            REQUIRE(cell.is_neighbor({5, 5}));
            REQUIRE(cell.is_neighbor({4, 4}));
            REQUIRE(cell.is_neighbor({4, 6}));

            REQUIRE_FALSE(cell.is_neighbor({3, 4}));
            REQUIRE_FALSE(cell.is_neighbor({3, 6}));
            REQUIRE_FALSE(cell.is_neighbor({5, 4}));
            REQUIRE_FALSE(cell.is_neighbor({5, 6}));
            REQUIRE_FALSE(cell.is_neighbor({4, 5}));

            REQUIRE_FALSE(cell.is_neighbor({1, 2}));
            REQUIRE_FALSE(cell.is_neighbor({-1, -2}));
            REQUIRE_FALSE(cell.is_neighbor({-3, -5}));
        }

        THEN("Cell (4, 5) is equal and any other cell is not") {
            REQUIRE(cell == twm::Cell(4, 5));
            REQUIRE_FALSE(cell != twm::Cell(4, 5));

            REQUIRE_FALSE(cell == twm::Cell(3, 5));
            REQUIRE(cell != twm::Cell(3, 5));
            REQUIRE_FALSE(cell == twm::Cell(4, 6));
            REQUIRE(cell != twm::Cell(4, 6));
        }
    }
}
