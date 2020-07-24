#include <twm/Action.h>

const twm::Action twm::Action::null_action = {twm::Cell::null_cell};

twm::Action::Action() {
    (*this) = null_action;
}

twm::Action::Action(const twm::Cell& cell) {
    this->cell = cell;
}

bool twm::operator==(const twm::Action& action, const twm::Action& other_action) {
    return action.cell == other_action.cell;
}

bool twm::operator!=(const twm::Action& action, const twm::Action& other_action) {
    return action.cell != other_action.cell;
}

bool twm::Action::is_null() const {
    return (*this) == twm::Action::null_action;
}
