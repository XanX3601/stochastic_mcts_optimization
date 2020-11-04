#include <mca/twm/nrpa/Policy.h>

#include <algorithm>
#include <cmath>
#include <random>

mca::twm::nrpa::Policy::Policy() {}

bool mca::twm::nrpa::Policy::does_contain(int code) const {
    return code_to_weight.find(code) != code_to_weight.end();
}

double mca::twm::nrpa::Policy::get_weight(int code) const {
    if (!does_contain(code)) {
        return 0;
    }

    return code_to_weight.at(code);
}

void mca::twm::nrpa::Policy::set_weight(int code, double weight) { code_to_weight[code] = weight; }

void mca::twm::nrpa::Policy::adapt(const Sequence& sequence, double learning_step) {
    Policy adapted_policy(*this);

    for (int move_index = 0; move_index < sequence.get_move_count(); ++move_index) {
        int code = sequence.get_chosen_code(move_index);

        adapted_policy.set_weight(code, adapted_policy.get_weight(code) + learning_step);

        auto possible_codes = sequence.get_possible_codes(move_index);

        double sum_weight = std::accumulate(
            possible_codes.begin(), possible_codes.end(), 0,
            [&](double sum, int code) { return std::move(sum) + std::exp(get_weight(code)); });

        std::for_each(possible_codes.begin(), possible_codes.end(), [&](int code) {
            adapted_policy.set_weight(code,
                                      adapted_policy.get_weight(code) -
                                          (learning_step * (exp(get_weight(code)) / sum_weight)));
        });
    }

    (*this) = adapted_policy;
}
