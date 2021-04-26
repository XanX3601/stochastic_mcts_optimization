#include <mca/twm/snrpa/code.h>
#include <mca/twm/snrpa/playout.h>

mca::twm::snrpa::Sequence mca::twm::snrpa::playout(const ::twm::Board& board, const Policy& policy,
                                                   int playout_count) {
    Sequence sequence = policy.generate_sequence();

    for (int playout_index = 0; playout_index < playout_count; ++playout_index) {
        sequence.add_score(playout(board, sequence));
    }

    return sequence;
}

int mca::twm::snrpa::playout(const ::twm::Board& board, const Sequence& sequence) {
    ::twm::Board playout_board(board);

    while (!playout_board.is_final()) {
        int code_index = 0;
        int code = sequence.get_code(code_index);

        while (!is_code_legal(playout_board, code)) {
            code_index++;
            code = sequence.get_code(code_index);
        }

        ::twm::Action chosen_action = decodify_action(board.get_problem(), code);
        playout_board = playout_board.get_next_board(chosen_action);
    }

    return playout_board.get_reward();
}
