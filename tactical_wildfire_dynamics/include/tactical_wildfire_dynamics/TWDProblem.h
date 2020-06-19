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
        std::vector<std::vector<double>> each_ignition_probability;
        std::vector<double> each_extinguishment_probability;
        std::vector<int> each_cell_burning_reward;

    public:
        TWDProblem(int grid_width, int grid_height);
        TWDProblem(int grid_width, int grid_height, std::vector<int> each_cell_initial_fuel_amount, std::vector<bool> is_each_cell_initially_burning, std::vector<std::vector<double>> each_ignition_probability, std::vector<double> each_extinguishment_probability, std::vector<int> each_cell_burning_reward);
        TWDProblem(const TWDProblem& problem);
        ~TWDProblem();

        TWDProblem& operator=(const TWDProblem& problem);

        int cell_to_index(const TWDCell& cell) const;
        TWDCell index_to_cell(int cell_index) const;

        int get_cell_initial_fuel_amount(const TWDCell& cell) const;
        bool is_cell_initially_burning(const TWDCell& cell) const;
        double get_probability_that_y_ignites_x(const TWDCell& x, const TWDCell& y) const;
        double get_probability_to_extinguish_cell(const TWDCell& cell) const;
        int get_cell_burning_reward(const TWDCell& cell) const;
};

#endif /* TACTICAL_WILDFIRE_DYNAMICS_PROBLEM_H */
