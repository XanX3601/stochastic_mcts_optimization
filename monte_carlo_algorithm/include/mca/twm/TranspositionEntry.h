#ifndef MCA_TWM_TRANSPOSITION_ENTRY_H
#define MCA_TWM_TRANSPOSITION_ENTRY_H

#include <bits/c++config.h>
#include <twm/Action.h>

#include <boost/functional/hash/hash.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace mca {
namespace twm {
class TranspositionEntry;

struct action_hash {
    std::size_t operator()(const ::twm::Action& action) const noexcept;
};
}  // namespace twm
}  // namespace mca

class mca::twm::TranspositionEntry {
   private:
    static const int number_of_playout_index;
    static const int accumulated_reward_index;

    unsigned long long hash_value;
    int number_of_playout;
    int accumulated_reward;
    std::unordered_map<::twm::Action, std::vector<int>, mca::twm::action_hash>
        action_to_info_vector;
    std::unordered_map<::twm::Action, std::unordered_set<unsigned long long>,
                       ::mca::twm::action_hash>
        action_to_next_hash_values;

    void insert(const ::twm::Action& action);

   public:
    TranspositionEntry(unsigned long long hash_value);

    unsigned long long get_hash_value() const;
    int get_number_of_playout() const;
    int get_action_number_of_playout(const ::twm::Action& action) const;
    int get_accumulated_reward() const;
    int get_action_accumulated_reward(const ::twm::Action& action) const;
    bool does_contain(const ::twm::Action& action) const;
    void update(const ::twm::Action& action, int reward, unsigned long long next_hash_value);
    int get_action_number_of_unique_hash_value(const ::twm::Action& action) const;
};

#endif /* MCA_TWM_TRANSPOSITION_ENTRY_H */
