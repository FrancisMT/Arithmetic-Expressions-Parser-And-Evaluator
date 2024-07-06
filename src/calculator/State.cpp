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

              //   TODO: Check how to fix cyclic dependencies: the following is not working.
              //   if (mOperandValuesMap.insert_or_assign(newOperand, newValue).second) {
              //       // They key "newOperand" didn't  exist in the map: delete a possible
              //       expression
              //       // with dependencies.
              //     //   if (mExpressionsWithDependenciesMap.contains(newOperand)) {
              //     //       mExpressionsWithDependenciesMap.erase(newOperand);
              //     //   }

              //       // This probably means that the we are being requested a value
              //       override.
              //       // Furthermore, we need to take into account cyclic dependencies:
              //       // What happens if we end-up with the following operation order?
              //       // -> 1. a = b | 2. b = a | 3. a = 1
              //       // To account for this, we do a "surgical override" and check
              //       // if the operand is currently assigned to an expression that
              //       // depends on another operand, and then remove it.

              //       // If the newOperand (that has a value assigned to it),
              //       // had previously an expression with dependencies assigned,
              //       // delete the expression. This will avoid cyclic dependencies.
              //       // (e.g. 1. a = b | 2. b = a | 3. c = a+b | 4.a = )

              //       //   MathUtils::Methods::removeEntryWithValue(mOperandDependenciesMap,
              //       //   newOperand);
              //   }

              affectedValues.emplace_back(newOperand, newValue);

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