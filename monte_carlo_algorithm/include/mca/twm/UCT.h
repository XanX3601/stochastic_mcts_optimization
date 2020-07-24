#ifndef MCA_TWM_UCT_H
#define MCA_TWM_UCT_H

#include "mca/twm/TranspositionTable.h"
#include <twm/Problem.h>
#include <twm/Board.h>

namespace mca {
    namespace twm {
        class UCT;
    }
}

class mca::twm::UCT
{
    private:
        ::twm::Board root_board;
        mca::twm::TranspositionTable transposition_table;
        double c;

        int playout(const ::twm::Board& board);

    public:
        UCT(::twm::Board board, double c);
        void search();
        int search(const ::twm::Board& board);
        TranspositionTable& get_transposition_table();
};

#endif /* MCA_TWM_UCT_H */
