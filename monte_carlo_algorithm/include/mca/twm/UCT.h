#ifndef MCA_TWM_UCT_H
#define MCA_TWM_UCT_H

#include <twm/Board.h>
#include <twm/Problem.h>

#include "mca/twm/TranspositionTable.h"

namespace mca {
namespace twm {
namespace uct {
class UCT {
   private:
    TranspositionTable transposition_table;
    double exploration_parameter;

   public:
    UCT(double exploration_parameter);
    int search(const ::twm::Board& board);
    static int playout(const ::twm::Board& board);
    TranspositionTable& get_transposition_table();
};
}  // namespace uct
}  // namespace twm
}  // namespace mca

#endif /* MCA_TWM_UCT_H */
