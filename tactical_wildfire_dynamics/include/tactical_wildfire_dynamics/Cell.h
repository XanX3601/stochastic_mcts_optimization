#ifndef TACTICAL_WILDFIRE_DYNAMICS_CELL_H
#define TACTICAL_WILDFIRE_DYNAMICS_CELL_H

namespace TWD {
    class Cell;

    bool operator==(const TWD::Cell& cell, const TWD::Cell& other_cell);
    bool operator!=(const TWD::Cell& cell, const TWD::Cell& other_cell);
}

class TWD::Cell {
    private:
    public:
        int x = 0;
        int y = 0;

        Cell(int x, int y);

        friend bool operator==(const TWD::Cell& cell, const TWD::Cell& other_cell);
        friend bool operator!=(const TWD::Cell& cell, const TWD::Cell& other_cell);

        bool is_neighbor(const TWD::Cell& cell);
};

#endif /* TACTICAL_WILDFIRE_DYNAMICS_CELL_H */
