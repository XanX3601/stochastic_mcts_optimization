#include <tactical_wildfire_dynamics/TWDCell.h>

TWDCell::TWDCell(int x, int y) {
    this->x = x;
    this->y = y;
}

TWDCell::TWDCell(const TWDCell& cell) {
    x = cell.x;
    y = cell.y;
}

TWDCell::~TWDCell() = default;

TWDCell& TWDCell::operator=(const TWDCell& cell) {
    if (this != &cell) {
        x = cell.x;
        y = cell.y;
    }

    return *this;
}
