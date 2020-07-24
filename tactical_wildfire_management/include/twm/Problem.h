#ifndef TWM_PROBLEM_H
#define TWM_PROBLEM_H

#include <twm/Cell.h>
#include <twm/Action.h>
#include <vector>
#include <unordered_map>

namespace twm {
    class Problem;
}

class twm::Problem {
    private:
        int grid_width;
        int grid_height;

        std::vector<int> each_cell_initial_fuel_amount;
        std::vector<bool> is_each_cell_initially_burning;
        std::vector<std::vector<double>> each_cell_ignition_probabilities;
        std::vector<double> each_cell_extinguishment_probability;
        std::vector<int> each_cell_burning_reward;
        int team_count = 0;

        std::unordered_map<int, std::vector<unsigned long long>> each_cell_fuel_random_bitstrings;
        std::unordered_map<int, std::vector<unsigned long long>> each_cell_burning_random_bitstrings;
        std::unordered_map<int, std::vector<unsigned long long>> each_cell_team_random_bitstrings;
        std::vector<unsigned long long> each_team_turn_random_bitstrings;

        std::vector<Action> legal_actions;

        void init_random_bitstrings();
        void init_legal_actions();

    public:
        Problem(int grid_width, int grid_height);
        Problem(int grid_width,
                int grid_height,
                const std::vector<int>& each_cell_initial_fuel_amount,
                const std::vector<bool>& is_each_cell_initially_burning,
                const std::vector<std::vector<double>>& each_cell_ignition_probabilities,
                const std::vector<double>& each_cell_extinguishment_probability,
                const std::vector<int> each_cell_burning_reward,
                int team_count);

        Problem(int grid_width,
                int grid_height,
                const std::vector<int>& each_cell_initial_fuel_amount,
                const std::vector<bool>& is_each_cell_initially_burning,
                double neighbors_ignition_probability,
                const std::vector<double>& each_cell_extinguishment_probability,
                const std::vector<int> each_cell_burning_reward,
                int team_count);

        int cell_to_index(const twm::Cell& cell) const;
        twm::Cell index_to_cell(int cell_index) const;

        int get_grid_width() const;
        int get_grid_height() const;
        int get_cell_count() const;
        int get_cell_initial_fuel_amount(const twm::Cell& cell) const;
        int get_cell_initial_fuel_amount(int cell_index) const;
        bool is_cell_initially_burning(const twm::Cell& cell) const;
        bool is_cell_initially_burning(int cell_index) const;
        double get_probability_that_y_ignites_x(const twm::Cell& x, const twm::Cell& y) const;
        double get_probability_that_y_ignites_x(int x_index, int y_index) const;
        double get_probability_to_extinguish_cell(const twm::Cell& cell) const;
        double get_probability_to_extinguish_cell(int cell_index) const;
        int get_cell_burning_reward(const twm::Cell& cell) const;
        int get_cell_burning_reward(int cell_index) const;
        int get_team_count() const;

        unsigned long long get_cell_fuel_random_bitstring(const twm::Cell& cell, int fuel_amount) const;
        unsigned long long get_cell_fuel_random_bitstring(int cell_index, int fuel_amount) const;
        unsigned long long get_cell_burning_random_bitstring(const twm::Cell& cell, bool is_cell_burning) const;
        unsigned long long get_cell_burning_random_bitstring(int cell_index, bool is_cell_burning) const;
        unsigned long long get_cell_team_random_bitstring(const twm::Cell& cell, int team_index) const;
        unsigned long long get_cell_team_random_bitstring(int cell_index, int team_index) const;
        unsigned long long get_team_turn_random_bitstring(int team_index) const;

        const std::vector<Action>& get_legal_actions() const;
};

#endif /* TWM_PROBLEM_H */
