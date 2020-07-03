#include <tactical_wildfire_dynamics/Cell.h>

#include <cmath>

TWD::Cell::Cell(int x, int y) {
    this->x = x;
    this->y = y;
}

bool TWD::operator==(const TWD::Cell& cell, const TWD::Cell& other_cell) {
    return cell.x == other_cell.x && cell.y == other_cell.y;
}

bool TWD::operator!=(const TWD::Cell& cell, const TWD::Cell& other_cell) {
    return cell.x != other_cell.x || cell.y != other_cell.y;
}

bool TWD::Cell::is_neighbor(const TWD::Cell& cell) {
    return abs(x - cell.x) + abs(y - cell.y) == 1;
}
