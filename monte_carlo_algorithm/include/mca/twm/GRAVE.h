#ifndef MCA_TWM_GRAVE_H
#define MCA_TWM_GRAVE_H

#include <mca/twm/TranspositionTable.h>
#include <twm/Board.h>
#include <twm/Problem.h>

#include <unordered_set>

namespace mca {
namespace twm {
namespace grave {
class GRAVE {
   private:
    TranspositionTable transposition_table;
    double bias;
    int ref;

   public:
    GRAVE(double bias, int ref);
    std::pair<int, std::unordered_set<::twm::Action, action_hash>> search(
        const ::twm::Board& board, TranspositionEntry& entry_ref);
    static std::pair<int, std::unordered_set<::twm::Action, action_hash>> playout(
        const ::twm::Board& board);
    TranspositionTable& get_transposition_table();
};
}  // namespace grave
}  // namespace twm
}  // namespace mca

#endif /* MCA_TWM_UCT_H */
