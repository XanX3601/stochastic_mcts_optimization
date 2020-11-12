#include <mca/twm/snrpa/Policy.h>

#include <cmath>
#include <random>

void mca::twm::snrpa::Policy::set_weight(int code, double weight) {
    double current_weight = code_to_weight[code];
    exp_weight_sum -= std::exp(current_weight);
    exp_weight_sum += std::exp(weight);
    code_to_weight[code] = weight;
}

mca::twm::snrpa::Policy::Policy(const std::unordered_set<int> codes) {
    exp_weight_sum = 0;

    for (int code : codes) {
        code_to_weight[code] = 0;
        exp_weight_sum += 1;
    }
}

void mca::twm::snrpa::Policy::adapt(const mca::twm::snrpa::Sequence& sequence,
                                    double learning_step) {
    double exp_weight_sum_adapt = exp_weight_sum;

    for (int adapt_index = 0; adapt_index < sequence.get_code_count() - 1; ++adapt_index) {
        double exp_weight_sum_adapt_copy = exp_weight_sum_adapt;

        int code_to_adapt = sequence.get_code(adapt_index);
        double weight = code_to_weight[code_to_adapt];

        exp_weight_sum_adapt_copy -= std::exp(weight);

        set_weight(code_to_adapt, weight + learning_step -
                                      learning_step * (std::exp(weight) / exp_weight_sum_adapt));

        for (int sequence_index = adapt_index + 1; sequence_index < sequence.get_code_count();
             ++sequence_index) {
            int code = sequence.get_code(sequence_index);
            weight = code_to_weight[code];

            exp_weight_sum_adapt_copy -= std::exp(weight);

            double new_weight = weight - learning_step * (std::exp(weight) / exp_weight_sum_adapt);
            set_weight(code, new_weight);
            exp_weight_sum_adapt_copy += std::exp(new_weight);
        }
        exp_weight_sum_adapt = exp_weight_sum_adapt_copy;
    }
}

double mca::twm::snrpa::Policy::get_code_weight(int code) const { return code_to_weight.at(code); }

mca::twm::snrpa::Sequence mca::twm::snrpa::Policy::generate_sequence() const {
    std::vector<int> sequence_codes(code_to_weight.size());
    std::unordered_set<int> sequence_codes_set;

    std::random_device rd;
    std::mt19937 gen(rd());

    double exp_weight_sum = this->exp_weight_sum;

    for (int code_index = 0; code_index < code_to_weight.size(); ++code_index) {
        std::uniform_real_distribution<double> dis(0, exp_weight_sum);

        double target = dis(gen);
        auto code_to_weight_entry = code_to_weight.cbegin();

        while (target > 0) {
            if (!sequence_codes_set.count(code_to_weight_entry->first)) {
                target -= std::exp(code_to_weight_entry->second);
            }

            if (target > 0) {
                code_to_weight_entry++;
            }
        }

        sequence_codes[code_index] = code_to_weight_entry->first;
        sequence_codes_set.insert(code_to_weight_entry->first);
        exp_weight_sum -= std::exp(code_to_weight_entry->second);
    }

    return {sequence_codes};
}
