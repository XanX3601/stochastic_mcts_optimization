#ifndef TWM_ACTION_H
#define TWM_ACTION_H

#include <twm/Cell.h>

namespace twm {
    class Action;

    bool operator==(const twm::Action& action, const twm::Action& other_action);
    bool operator!=(const twm::Action& action, const twm::Action& other_action);
}

class twm::Action
{
    private:

    public:
        static const twm::Action null_action;

        twm::Cell cell {0, 0};

        Action();
        Action(const twm::Cell& cell);

        friend bool operator==(const twm::Action& action, const twm::Action& other_action);
        friend bool operator!=(const twm::Action& action, const twm::Action& other_action);

        bool is_null() const;
};

#endif /* TWM_ACTION_H */
