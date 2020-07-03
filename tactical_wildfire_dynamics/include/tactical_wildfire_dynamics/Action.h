#ifndef TACTICAL_WILDFIRE_DYNAMICS_ACTION_H
#define TACTICAL_WILDFIRE_DYNAMICS_ACTION_H

#include <tactical_wildfire_dynamics/Cell.h>

namespace TWD {
    class Action;
}

class TWD::Action
{
    private:


    public:
        TWD::Cell cell {0, 0};

        Action(const TWD::Cell& cell);
};

#endif /* TACTICAL_WILDFIRE_DYNAMICS_ACTION_H */
