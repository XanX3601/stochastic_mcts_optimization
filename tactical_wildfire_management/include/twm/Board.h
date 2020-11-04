#ifndef TWM_BOARD_H
#define TWM_BOARD_H

#include <twm/Action.h>
#include <twm/Cell.h>
#include <twm/Problem.h>

#include <vector>

namespace twm {
class Board;
}

class twm::Board {
   private:
    twm::Problem* problem;
    std::vector<int> each_cell_fuel_amount;
    std::vector<bool> is_each_cell_burning;
    std::vector<twm::Cell> each_team_cell;
    int next_team_index_to_play;
    int reward;
    int burning_cell_count;
    unsigned long long hash_value;

   public:
    Board(twm::Problem* problem);

    int get_cell_fuel_amount(const twm::Cell& cell) const;
    int get_cell_fuel_amount(int cell_index) const;
    bool is_cell_burning(const twm::Cell& cell) const;
    bool is_cell_burning(int cell_index) const;
    twm::Cell get_team_cell(int team_index) const;
    int get_next_team_index_to_play() const;
    int get_reward() const;
    bool is_action_legal(const twm::Action& action) const;
    twm::Board get_next_board(const twm::Action& action) const;
    bool is_final() const;
    int get_burning_cell_count() const;
    unsigned long long get_hash_value() const;
    const std::vector<twm::Action>& get_legal_actions() const;
    void reset_reward();
    void scale_fuel_amount(double scale);
    int lowest_possible_reward() const;
    int highest_possible_reward() const;
    Problem const& get_problem() const;
};

#endif /* TWM_BOARD_H */
