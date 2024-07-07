#include "State.hpp"

#include <functional>

#include "evaluator/Evaluator.hpp"
#include "mathUtils/Methods.hpp"

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
    std::vector<std::pair<std::string, int>> affectedValues;

    // Helper function to handle the recursive logic
    std::function<void(const std::string&, int)> storeValueAndCheckDependencies =

          [&](const std::string& newOperand, int newValue) {
              mOperandValuesMap.insert_or_assign(newOperand, newValue);

              affectedValues.emplace_back(newOperand, newValue);

              // Check if there are any expressions that depend on the provided operand
              // (whose value is now known) and if so, try to resolve them.
              const auto operandDependencyRange = mOperandDependenciesMap.equal_range(newOperand);

              for (auto itr = operandDependencyRange.first; itr != operandDependencyRange.second;
                   ++itr) {

                  const auto dependantOperand = itr->second;

                  if (mExpressionsWithDependenciesMap.contains(dependantOperand)) {

                      Evaluator evaluator(
                            mExpressionsWithDependenciesMap.at(dependantOperand).getASTOfRHS(),
                            mOperandValuesMap);
                      const auto evaluatorResult = evaluator.execute();

                      if (std::holds_alternative<int>(evaluatorResult)) {
                          const auto dependantOperandResult = std::get<int>(evaluatorResult);
                          storeValueAndCheckDependencies(dependantOperand, dependantOperandResult);
                      }
                  }
              }
          };

    // Initial call to start the recursive dependencies checking.
    storeValueAndCheckDependencies(operand, value);

    return affectedValues;
}

bool State::storeExpressionDependencies(const std::string& operand,
                                        Parser&& parser,
                                        const std::unordered_set<std::string>& dependencies)
{

    // Check for cyclic dependencies (e.g. 1. a = c | 2. b = a | 3. c = a):
    // - we check if there is already an entry in the operand dependencies map
    // where a [operand, dependency] pair exists.
    const auto operandDependencyRange = mOperandDependenciesMap.equal_range(operand);

    for (auto itr = operandDependencyRange.first; itr != operandDependencyRange.second; ++itr) {

        const auto dependantOperand = itr->second;

        if (dependencies.contains(dependantOperand)) {
            std::cout << "Cyclic dependency found: \'" << operand << "\' depends on \'"
                      << dependantOperand << "\' but \'" << dependantOperand << "\' already depends on \'"
                      << operand << "\'\n";
            return false;
        }
    }

    // Store the parser containing the expression for the provided operand
    // since it might be resolved later if the dependencies are met.
    mExpressionsWithDependenciesMap.insert_or_assign(operand, std::forward<Parser>(parser));

    // Add the new dependencies to the operand dependencies map
    // so that the operand becomes dependent on each of them.
    for (const auto& dependency : dependencies) {
        mOperandDependenciesMap.emplace(dependency, operand);
    }

    return true;
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