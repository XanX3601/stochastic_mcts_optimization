#ifndef MCA_TWM_SNRPA_RESULT
#define MCA_TWM_SNRPA_RESULT

#include <mca/twm/snrpa/Sequence.h>

#include <string>

namespace mca {
namespace twm {
namespace snrpa {

class Result {
   private:
    bool closed;
    Sequence sequence;
    std::vector<Sequence> root_level_sequences;

   public:
    Result();
    Sequence get_sequence() const;
    void set_sequence(const Sequence& sequence);
    void close();
    void add_root_sequence(const Sequence& sequence);
    std::vector<Sequence> get_root_level_sequences() const;
    void save_to_csv(std::string file_path) const;
};

}  // namespace snrpa
}  // namespace twm
}  // namespace mca

#endif /* MCA_TWM_SNRPA_RESULT */
