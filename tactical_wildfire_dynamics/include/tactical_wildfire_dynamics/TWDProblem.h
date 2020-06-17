#ifndef TACTICAL_WILDFIRE_DYNAMICS_PROBLEM_H
#define TACTICAL_WILDFIRE_DYNAMICS_PROBLEM_H

#include <tactical_wildfire_dynamics/TWDCell.h>
#include <vector>
class TWDProblem {
    private:
        int grid_width;
        int grid_height;

        std::vector<int> each_cell_initial_fuel_amount;
        std::vector<bool> is_each_cell_initially_burning;

    public:
        TWDProblem(int grid_width, int grid_height);
        TWDProblem(const TWDProblem& problem);
        virtual ~TWDProblem();

        TWDProblem& operator=(const TWDProblem& problem);

        int cell_to_index(const TWDCell& cell);
        TWDCell index_to_cell(int cell_index);
};

#endif /* TACTICAL_WILDFIRE_DYNAMICS_PROBLEM_H */
