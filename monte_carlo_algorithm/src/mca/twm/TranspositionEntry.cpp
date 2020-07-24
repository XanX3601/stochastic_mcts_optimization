#include "twm/Action.h"
#include <bits/c++config.h>
#include <boost/container_hash/hash_fwd.hpp>
#include <mca/twm/TranspositionEntry.h>
#include <vector>

std::size_t mca::twm::action_hash::operator()(const ::twm::Action& action) const noexcept {
    std::size_t seed = 0;
    boost::hash_combine(seed, action.cell.x);
    boost::hash_combine(seed, action.cell.y);
    return seed;
}

const int mca::twm::TranspositionEntry::number_of_playout_index = 0;
const int mca::twm::TranspositionEntry::accumulated_reward_index = 1;

void mca::twm::TranspositionEntry::insert(const ::twm::Action& action) {
    action_to_info_vector[action] = std::vector<int>(2, 0);
}

mca::twm::TranspositionEntry::TranspositionEntry(unsigned long long hash_value) : hash_value(hash_value) {
    number_of_playout = 0;
    accumulated_reward = 0;
}

unsigned long long mca::twm::TranspositionEntry::get_hash_value() const {
    return hash_value;
}

int mca::twm::TranspositionEntry::get_number_of_playout() const {
    return number_of_playout;
}

int mca::twm::TranspositionEntry::get_action_number_of_playout(const ::twm::Action& action) const {
    return action_to_info_vector.at(action)[number_of_playout_index];
}

int mca::twm::TranspositionEntry::get_accumulated_reward() const {
    return accumulated_reward;
}

int mca::twm::TranspositionEntry::get_action_accumulated_reward(const ::twm::Action& action) const {
    return action_to_info_vector.at(action)[accumulated_reward_index];
}

bool mca::twm::TranspositionEntry::does_contain(const ::twm::Action& action) const {
    return action_to_info_vector.find(action) != action_to_info_vector.end();
}

void mca::twm::TranspositionEntry::update(const ::twm::Action& action, int reward) {
    number_of_playout++;
    accumulated_reward += reward;

    if (!does_contain(action)) {
        insert(action);
    }

    action_to_info_vector[action][number_of_playout_index]++;
    action_to_info_vector[action][accumulated_reward_index] += reward;
}
