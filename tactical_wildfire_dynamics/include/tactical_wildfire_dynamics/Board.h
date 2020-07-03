#ifndef TACTICAL_WILDFIRE_DYNAMICS_BOARD_H
#define TACTICAL_WILDFIRE_DYNAMICS_BOARD_H

#include <tactical_wildfire_dynamics/Cell.h>
#include <tactical_wildfire_dynamics/Problem.h>
#include <tactical_wildfire_dynamics/Action.h>
#include <vector>

namespace TWD {
    class Board;
}

class TWD::Board
{
    private:
        TWD::Problem* problem;
        std::vector<int> each_cell_fuel_amount;
        std::vector<bool> is_each_cell_burning;
        std::vector<TWD::Cell> each_team_cell;
        int next_team_index_to_play;
        int reward;
        int burning_cell_count;
        unsigned long long hash_value;

    public:
        Board(TWD::Problem* problem);

        int get_cell_fuel_amount(const TWD::Cell& cell) const;
        int get_cell_fuel_amount(int cell_index) const;
        bool is_cell_burning(const TWD::Cell& cell) const;
        bool is_cell_burning(int cell_index) const;
        TWD::Cell get_team_cell(int team_index) const;
        int get_next_team_index_to_play() const;
        int get_reward() const;
        bool is_action_legal(const TWD::Action& action) const;
        TWD::Board get_next_board(const TWD::Action& action) const;
        bool is_final() const;
        int get_burning_cell_count() const;
};

#endif /* TACTICAL_WILDFIRE_DYNAMICS_BOARD_H */
