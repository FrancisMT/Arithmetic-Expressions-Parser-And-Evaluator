#include "State.hpp"

#include "evaluator/Evaluator.hpp"

namespace Calculator {

State::State() {}

State::~State() {}

void State::updateOperationOrder(const std::string& operand)
{
    mOperandOrderStack.push(operand);
}

std::vector<std::pair<std::string, int>>
      State::storeExpressionValue(const std::string& operand, const int value)
{

    mOperandValuesMap.insert_or_assign(operand, value);

    std::vector<std::pair<std::string, int>> affectedValues{{operand, value}};

    // TODO: This needs to be recursive!!!! -> a=b-2 || b = c + 8 || c = 2+2

    // Check if we can calculate the value of operands
    // that depend on the value of the one that was just stored.
    const auto operandDependencyRange = mOperandDependenciesMap.equal_range(operand);
    for (auto itr = operandDependencyRange.first; itr != operandDependencyRange.second; ++itr) {

        const auto dependantOperand = itr->second;

        if (mExpressionsWithDependenciesMap.contains(dependantOperand)) {

            // AST::printAST(mExpressionsWithDependenciesMap.at(dependantOperand).getASTOfRHS());
            Evaluator evaluator(mExpressionsWithDependenciesMap.at(dependantOperand).getASTOfRHS(),
                                mOperandValuesMap);
            const auto evaluatorResult = evaluator.execute();

            if (std::holds_alternative<int>(evaluatorResult)) {

                const auto dependantOperandResult = std::get<int>(evaluatorResult);
                mOperandValuesMap.insert_or_assign(dependantOperand, dependantOperandResult);

                affectedValues.emplace_back(dependantOperand, dependantOperandResult);
            }
        }
    }

    return affectedValues;
}

void State::storeExpressionDependencies(const std::string& operand,
                                        Parser&& parser,
                                        const std::unordered_set<std::string>& dependencies)
{
    mExpressionsWithDependenciesMap.insert_or_assign(operand, std::forward<Parser>(parser));

    for (const auto& dependency : dependencies) {
        mOperandDependenciesMap.emplace(dependency, operand);
    }
}

const std::unordered_map<std::string, int>& State::getOperandValueMap() const
{
    return mOperandValuesMap;
}

std::pair<std::string, int> State::getLastFulfilledOperation() const
{
    // Go through the stack of operations history and check if the corresponding operand
    // already has a value available.
    auto operandOrderStack = mOperandOrderStack;
    while (!operandOrderStack.empty()) {

        if (mOperandValuesMap.contains(operandOrderStack.top())) {

            return {operandOrderStack.top(), mOperandValuesMap.at(operandOrderStack.top())};
        }

        operandOrderStack.pop();
    }

    return {};
}

std::vector<std::string> State::undoLastRegisteredOperations(const int undoCount)
{
    std::vector<std::string> deletedOperations;

    if (undoCount <= 0) {
        return deletedOperations;
    }

    for (int deleteCounter = 0; deleteCounter < undoCount; ++deleteCounter) {

        // We delete the necessary value from the operations history stack.
        const auto operand = mOperandOrderStack.top();

        // We check if the we need to either delete the entry in the operandValuesMap or
        // the expressionsWithDependenciesMap
        if (mOperandValuesMap.contains(operand)) {
            mOperandValuesMap.erase(operand);
        }

        if (mExpressionsWithDependenciesMap.contains(operand)) {
            mExpressionsWithDependenciesMap.erase(operand);
        }

        // Finally, delete the operand from the history stack.
        mOperandOrderStack.pop();

        deletedOperations.push_back(operand);
    }

    return deletedOperations;
}

} // namespace Calculator