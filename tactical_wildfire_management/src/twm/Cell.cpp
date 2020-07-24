#include <twm/Cell.h>

#include <cmath>

const twm::Cell twm::Cell::null_cell = {-1, -1};

twm::Cell::Cell(int x, int y) {
    this->x = x;
    this->y = y;
}

bool twm::operator==(const twm::Cell& cell, const twm::Cell& other_cell) {
    return cell.x == other_cell.x && cell.y == other_cell.y;
}

bool twm::operator!=(const twm::Cell& cell, const twm::Cell& other_cell) {
    return cell.x != other_cell.x || cell.y != other_cell.y;
}

bool twm::Cell::is_neighbor(const twm::Cell& cell) const {
    return abs(x - cell.x) + abs(y - cell.y) == 1;
}

bool twm::Cell::is_null() const {
    return (*this) == null_cell;
}
