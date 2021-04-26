#include <fmt/os.h>
#include <mca/twm/snrpa/Result.h>

#include <stdexcept>

mca::twm::snrpa::Result::Result() : closed(false), sequence({}) {}

mca::twm::snrpa::Sequence mca::twm::snrpa::Result::get_sequence() const { return sequence; }

void mca::twm::snrpa::Result::set_sequence(const Sequence& sequence) {
    if (closed) {
        throw std::invalid_argument("Result is closed and therefore can not be written upon");
    }

    this->sequence = sequence;
}

void mca::twm::snrpa::Result::close() { closed = true; }

void mca::twm::snrpa::Result::add_root_sequence(const Sequence& sequence) {
    if (closed) {
        throw std::invalid_argument("Result is closed and therefore can not be written upon");
    }

    root_level_sequences.push_back(sequence);
}

std::vector<mca::twm::snrpa::Sequence> mca::twm::snrpa::Result::get_root_level_sequences() const {
    return root_level_sequences;
}

void mca::twm::snrpa::Result::save_to_csv(std::string file_path) const {
    auto file = fmt::output_file(file_path);

    int code_count = sequence.get_code_count();

    file.print("mean_score;");
    for (int code_index = 0; code_index < code_count - 1; ++code_index) {
        file.print("{};", code_index);
    }
    file.print("{}\n", code_count - 1);

    for (Sequence sequence : root_level_sequences) {
        file.print("{};", sequence.get_mean_score());

        for (int code_index = 0; code_index < code_count - 1; ++code_index) {
            file.print("{};", sequence.get_code(code_index));
        }
        file.print("{}\n", sequence.get_code(code_count - 1));
    }
}
