#include <mca/twm/snrpa/Policy.h>
#include <mca/twm/snrpa/code.h>

#include <cmath>
#include <random>

void mca::twm::snrpa::Policy::set_weight(int code, double weight) {
    double current_weight = code_weights[code_to_index(code)];
    exp_weight_sum -= std::exp(current_weight);
    exp_weight_sum += std::exp(weight);
    code_weights[code_to_index(code)] = weight;
}

mca::twm::snrpa::Policy::Policy(int code_count) : code_weights(code_count, 0) {
    exp_weight_sum = code_count;
}

void mca::twm::snrpa::Policy::adapt(const mca::twm::snrpa::Sequence& sequence,
                                    double learning_step) {
    double exp_weight_sum_adapt = exp_weight_sum;

    for (int adapt_index = 0; adapt_index < sequence.get_code_count() - 1; ++adapt_index) {
        double exp_weight_sum_adapt_copy = exp_weight_sum_adapt;

        int code_to_adapt = sequence.get_code(adapt_index);
        double weight = code_weights[code_to_index(code_to_adapt)];

        exp_weight_sum_adapt_copy -= std::exp(weight);

        set_weight(code_to_adapt, weight + learning_step -
                                      learning_step * (std::exp(weight) / exp_weight_sum_adapt));

        for (int sequence_index = adapt_index + 1; sequence_index < sequence.get_code_count();
             ++sequence_index) {
            int code = sequence.get_code(sequence_index);
            weight = code_weights[code_to_index(code)];

            exp_weight_sum_adapt_copy -= std::exp(weight);

            double new_weight = weight - learning_step * (std::exp(weight) / exp_weight_sum_adapt);
            set_weight(code, new_weight);
            exp_weight_sum_adapt_copy += std::exp(new_weight);
        }
        exp_weight_sum_adapt = exp_weight_sum_adapt_copy;
    }
}

double mca::twm::snrpa::Policy::get_code_weight(int code) const {
    return code_weights[code_to_index(code)];
}

#include <iostream>

mca::twm::snrpa::Sequence mca::twm::snrpa::Policy::generate_sequence() const {
    std::vector<int> sequence_codes(code_weights.size());
    std::unordered_set<int> sequence_codes_set;

    for (int sequence_index = 0; sequence_index < sequence_codes.size(); ++sequence_index) {
        std::vector<double> cumulative_weights(sequence_codes.size());
    }
}
