#ifndef MCA_TWM_UCT_H
#define MCA_TWM_UCT_H

#include <twm/Board.h>
#include <twm/Problem.h>

#include "mca/twm/TranspositionTable.h"

namespace mca {
namespace twm {
int uct(const ::twm::Board& board, ::mca::twm::TranspositionTable& transposition_table, double c);
int uct_playout(const ::twm::Board& board);
}  // namespace twm
}  // namespace mca

#endif /* MCA_TWM_UCT_H */
