#ifndef TACTICAL_WILDFIRE_DYNAMICS_CELL_H
#define TACTICAL_WILDFIRE_DYNAMICS_CELL_H

class TWDCell {
    private:
    public:
        int x, y;

        TWDCell(int x, int y);
        TWDCell(const TWDCell& cell);
        ~TWDCell();

        TWDCell& operator=(const TWDCell& cell);
};

#endif /* TACTICAL_WILDFIRE_DYNAMICS_CELL_H */
