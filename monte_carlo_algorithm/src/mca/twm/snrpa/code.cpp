#include <mca/twm/snrpa/code.h>

int mca::twm::snrpa::compute_code_count(const ::twm::Problem& problem) {
    int biggest_cell_index = problem.get_cell_count() - 1;
    int team_count = problem.get_team_count();
    int cell_count = problem.get_cell_count();

    return 2 + biggest_cell_index + (team_count - 1 + team_count) * cell_count;
}

std::unordered_set<int> mca::twm::snrpa::compute_all_codes(const ::twm::Problem& problem) {
    std::unordered_set<int> codes;

    int biggest_cell_index = problem.get_cell_count() - 1;
    int team_count = problem.get_team_count();
    int cell_count = problem.get_cell_count();

    int biggest_code = biggest_cell_index + (team_count - 1 + team_count) * cell_count;

    for (int code = -1; code < biggest_code + 1; ++code) {
        codes.insert(code);
    }

    return codes;
}

int mca::twm::snrpa::codify_action(const ::twm::Board& board, const ::twm::Action& action) {
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

::twm::Action mca::twm::snrpa::decodify_action(const ::twm::Problem& problem, int code) {
    if (code == -1) {
        return ::twm::Action::null_action;
    }

    return {problem.index_to_cell(code % problem.get_cell_count())};
}

bool mca::twm::snrpa::is_code_legal(const ::twm::Board& board, int code) {
    ::twm::Action action = decodify_action(board.get_problem(), code);

    if (action.is_null()) {
        return true;
    }

    if (!board.is_action_legal(action)) {
        return false;
    }

    int target_team_index = get_target_team_index(board.get_problem(), code);

    if (board.get_next_team_index_to_play() != target_team_index) {
        return false;
    }

    if (does_code_target_burning_cell(board.get_problem(), code)) {
        return board.is_cell_burning(action.cell);
    }
    return !board.is_cell_burning(action.cell);
}

bool mca::twm::snrpa::does_code_target_burning_cell(const ::twm::Problem& problem, int code) {
    return code >= problem.get_cell_count() * problem.get_team_count();
}

int mca::twm::snrpa::get_target_team_index(const ::twm::Problem& problem, int code) {
    if (code < 0) {
        return -1;
    }

    int half_code = problem.get_team_count() * problem.get_cell_count();

    if (code >= half_code) {
        code -= half_code;
    }

    return code / problem.get_cell_count();
}

int mca::twm::snrpa::code_to_index(int code) { return code + 1; }

int mca::twm::snrpa::index_to_code(int code_index) { return code_index - 1; }
