#ifndef TWM_CELL_H
#define TWM_CELL_H

namespace twm {
    class Cell;

    bool operator==(const twm::Cell& cell, const twm::Cell& other_cell);
    bool operator!=(const twm::Cell& cell, const twm::Cell& other_cell);
}

class twm::Cell {
    private:
    public:
        static const twm::Cell null_cell;

        int x = 0;
        int y = 0;

        Cell(int x, int y);

        friend bool operator==(const twm::Cell& cell, const twm::Cell& other_cell);
        friend bool operator!=(const twm::Cell& cell, const twm::Cell& other_cell);

        bool is_neighbor(const twm::Cell& cell) const;
        bool is_null() const;
};

#endif /* TWM_CELL_H */
