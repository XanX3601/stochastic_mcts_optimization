#include <mca/twm/nrpa/code.h>

int mca::twm::nrpa::codify_action(const ::twm::Board& board, const ::twm::Action& action) {
    if (action.is_null()) {
        return -1;
    }

    auto problem = board.get_problem();
    int cell_index = problem.cell_to_index(action.cell);
    int team_index = board.get_next_team_index_to_play();
    int team_count = problem.get_team_count();
    int cell_count = problem.get_cell_count();
    bool is_cell_burning = board.is_cell_burning(cell_index);

    return cell_index + (team_index + (is_cell_burning ? team_count : 0)) * cell_count;
}

::twm::Action mca::twm::nrpa::decodify_action(const ::twm::Problem& problem, int code) {
    if (code == -1) {
        return ::twm::Action::null_action;
    }

    return {problem.index_to_cell(code % problem.get_cell_count())};
}
