#pragma once

#include <stack>
#include <string>
#include <vector>

#include "parser/Parser.hpp"
#include <unordered_map>
#include <unordered_set>

namespace Calculator {

class State
{
public:
    State();
    ~State();

    std::pair<std::string, int> getLastFulfilledOperation() const;

    std::vector<std::string> undoLastRegisteredOperations(const int undoCount);

private:
    // LIFO stack to keep order of the operations by keeping track of the operands of each
    // expression.
    std::stack<std::string> operandOrderStack;

    // Map holding the operands with already available values.
    std::unordered_map<std::string, int> operandValuesMap;

    // Map used to keep track of the dependencies between operands. This is a one to many
    // association since they key is the dependency and the value is a list of operands that
    // depend on the operand defined by the key.
    std::unordered_multimap<std::string, std::string> operandDependenciesMap;

    // Map used to keep track of arithmetic expressions that depend on the value of other
    // operands.
    std::unordered_map<std::string, Parser> expressionsWithDependenciesMap;
};

} // namespace Calculator