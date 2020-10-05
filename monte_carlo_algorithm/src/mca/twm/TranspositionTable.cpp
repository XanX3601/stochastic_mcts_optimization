#include <mca/twm/TranspositionEntry.h>
#include <mca/twm/TranspositionTable.h>

bool mca::twm::TranspositionTable::does_contain(unsigned long long hash_value) const {
    return hash_value_to_entry.find(hash_value) != hash_value_to_entry.end();
}

void mca::twm::TranspositionTable::insert(unsigned long long hash_value) {
    hash_value_to_entry.insert({hash_value, mca::twm::TranspositionEntry(hash_value)});
}

mca::twm::TranspositionEntry mca::twm::TranspositionTable::get_entry(
    unsigned long long hash_value) {
    return hash_value_to_entry.at(hash_value);
}

int mca::twm::TranspositionTable::get_number_of_entries() const {
    return hash_value_to_entry.size();
}

void mca::twm::TranspositionTable::update(unsigned long long hash_value,
                                          const ::twm::Action& action, int reward,
                                          unsigned long long next_hash_value) {
    hash_value_to_entry.at(hash_value).update(action, reward, next_hash_value);
}
