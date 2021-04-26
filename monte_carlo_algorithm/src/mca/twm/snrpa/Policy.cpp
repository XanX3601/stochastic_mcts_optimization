#include <mca/twm/snrpa/Policy.h>
#include <mca/twm/snrpa/code.h>

#include <cmath>
#include <iostream>
#include <random>

mca::twm::snrpa::Policy::Policy(int code_count)
    : each_code_weight(code_count, 0), code_count(code_count) {}

void mca::twm::snrpa::Policy::adapt(const mca::twm::snrpa::Sequence& sequence,
                                    double learning_step) {
    for (int sequence_index = 0; sequence_index < sequence.get_code_count() - 1; ++sequence_index) {
        // compute exp weight sum
        double exp_weight_sum = 0;
        for (int sequence_index_bis = sequence_index;
             sequence_index_bis < sequence.get_code_count(); ++sequence_index_bis) {
            int code_index = code_to_index(sequence.get_code(sequence_index_bis));
            double exp_weight = std::exp(each_code_weight[code_index]);
            exp_weight_sum += exp_weight;
        }

        int code_index = code_to_index(sequence.get_code(sequence_index));
        each_code_weight[code_index] =
            (each_code_weight[code_index] + learning_step) -
            learning_step * (std::exp(each_code_weight[code_index]) / exp_weight_sum);

        for (int sequence_index_bis = sequence_index + 1;
             sequence_index_bis < sequence.get_code_count(); ++sequence_index_bis) {
            code_index = code_to_index(sequence.get_code(sequence_index_bis));
            each_code_weight[code_index] =
                each_code_weight[code_index] -
                learning_step * (std::exp(each_code_weight[code_index]) / exp_weight_sum);
        }
    }

    for (double weight : each_code_weight) {
        if (std::isinf(weight)) {
            std::cout << "INF ALERT" << std::endl;
        }
    }
}

double mca::twm::snrpa::Policy::get_code_weight(int code) const {
    return each_code_weight[code_to_index(code)];
}

mca::twm::snrpa::Sequence mca::twm::snrpa::Policy::generate_sequence() const {
    std::vector<int> sequence_codes(code_count);
    std::unordered_set<int> sequence_code_indexes;
    std::vector<double> cumulative_weights(code_count);

    // compute cumulative weights
    cumulative_weights[0] = std::exp(each_code_weight[0]);
    for (int code_index = 1; code_index < code_count; ++code_index) {
        cumulative_weights[code_index] =
            cumulative_weights[code_index - 1] + std::exp(each_code_weight[code_index]);
    }

    std::random_device random_device;
    std::mt19937 random_generator(random_device());

    for (int sequence_index = 0; sequence_index < code_count; ++sequence_index) {
        // select a code index based on exponential of weights
        std::uniform_real_distribution<double> random_distribution(
            0, cumulative_weights[code_count - 1]);

        double target_distance = random_distribution(random_generator);
        int low = 0;
        int high = code_count;
        bool finished = false;

        while (!finished) {
            int mid = (low + high) / 2;
            double distance = cumulative_weights[mid];

            if (distance < target_distance) {
                low = mid + 1;
            } else if (distance > target_distance) {
                high = mid;
            } else {
                low = mid;
                finished = true;
            }

            finished = finished || low >= high;
        }

        // update sequence
        int chosen_code_index = low;
        sequence_codes[sequence_index] = index_to_code(chosen_code_index);
        sequence_code_indexes.insert(chosen_code_index);

        // update cumulative weights
        if (chosen_code_index == 0) {
            cumulative_weights[0] = 0;
        } else {
            cumulative_weights[chosen_code_index] = cumulative_weights[chosen_code_index - 1];
        }

        for (int code_index = chosen_code_index + 1; code_index < code_count; ++code_index) {
            if (sequence_code_indexes.find(code_index) != sequence_code_indexes.end()) {
                cumulative_weights[code_index] = cumulative_weights[code_index - 1];
            } else {
                cumulative_weights[code_index] =
                    cumulative_weights[code_index - 1] + std::exp(each_code_weight[code_index]);
            }
        }
    }

    return {sequence_codes};
}
