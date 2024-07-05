#include "State.hpp"

namespace Calculator {

State::State() {}

State::~State() {}

std::pair<std::string, int> State::getLastFulfilledOperation() const {}

std::vector<std::string> State::undoLastRegisteredOperations(const int undoCount) {}

} // namespace Calculator