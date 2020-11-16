#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <random>
#include <vector>

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0, 10);

    int weight_count = 10;
    std::vector<int> codes(weight_count);
    std::vector<int> indexes(weight_count);
    std::vector<double> weights(weight_count);
    std::vector<double> partial_weight_sum(weight_count);

    for (int weight_index = 0; weight_index < weight_count; ++weight_index) {
        indexes[weight_index] = weight_index;
        weights[weight_index] = dist(gen);
    }

    std::sort(indexes.begin(), indexes.end(),
              [&](int index_a, int index_b) { return weights[index_a] > weights[index_b]; });

    partial_weight_sum[0] = weights[indexes[0]];
    for (int index_index = 1; index_index < weight_count; ++index_index) {
        partial_weight_sum[index_index] =
            partial_weight_sum[index_index - 1] + weights[indexes[index_index]];
    }

    fmt::print("weights: {}\n", weights);
    fmt::print("indexes: {}\n", indexes);
    fmt::print("partial weight sum: {}\n", partial_weight_sum);

    std::uniform_real_distribution<double> random_dist(0, partial_weight_sum[weight_count - 1]);
    double target = random_dist(gen);

    int guess_index = 0;
    bool finished = false;

    while (!finished) {
        if (partial_weight_sum[guess_index] > target) {
            finished = true;
        } else {
            double weight = weights[indexes[guess_index]];
            double hop_distance = target - partial_weight_sum[guess_index];
            int hop_indices = 1 + int(hop_distance / weight);
            guess_index += hop_indices;
        }
    }

    fmt::print("target: {}\n", target);
    fmt::print("guess index: {}\n", guess_index);
    fmt::print("chosen index: {}\n", indexes[guess_index]);
}
