#ifndef MCA_TWM_TRANSPOSITION_TABLE_H
#define MCA_TWM_TRANSPOSITION_TABLE_H

#include <mca/twm/TranspositionEntry.h>

#include <iterator>
#include <unordered_map>
namespace mca {
namespace twm {
class TranspositionTable;
}
}  // namespace mca

class mca::twm::TranspositionTable {
   private:
    std::unordered_map<unsigned long long, mca::twm::TranspositionEntry> hash_value_to_entry;

   public:
    bool does_contain(unsigned long long hash_value) const;
    void insert(unsigned long long hash_value);
    mca::twm::TranspositionEntry get_entry(unsigned long long hash_value);
    int get_number_of_entries() const;
    void update(unsigned long long hash_value, const ::twm::Action& action, int reward,
                unsigned long long next_hash_value);
};

#endif /* MCA_TWM_TRANSPOSITION_TABLE_H */
