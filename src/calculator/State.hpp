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

    // void printDebug()
    // {
    //     std::cout << "\n operandOrderStack\n";
    //     auto stack = mOperandOrderStack;
    //     while (!stack.empty()) {
    //         std::cout << stack.top() << std::endl;
    //         stack.pop();
    //     }

    //     std::cout << "\n operandValuesMap\n";
    //     for (const auto& pair : mOperandValuesMap) {
    //         std::cout << pair.first << ": " << pair.second << std::endl;
    //     }

    //     std::cout << "\n operandDependenciesMap\n";
    //     for (const auto& pair : mOperandDependenciesMap) {
    //         std::cout << pair.first << ": " << pair.second << std::endl;
    //     }

    //     std::cout << "\n expressionsWithDependenciesMap\n";
    //     for (const auto& pair : mExpressionsWithDependenciesMap) {
    //         std::cout << pair.first << ": \n";
    //         AST::printAST(pair.second.getASTOfRHS());
    //     }
    // }

    void updateOperationOrder(const std::string& operand);

    // Every time we get a new value from an expression, check if it resolves any pending
    // operation.
    std::vector<std::pair<std::string, int>>
          storeExpressionValue(const std::string& operand, const int value);

    void storeExpressionDependencies(const std::string& operand,
                                     Parser&& parser,
                                     const std::unordered_set<std::string>& dependencies);

    const std::unordered_map<std::string, int>& getOperandValueMap() const;

    // Return the result of the last requested operation that has already been fulfilled.
    [[nodiscard]] std::pair<std::string, int> getLastFulfilledOperation() const;

    // Undo the last requested amount of operations.
    [[nodiscard]] std::vector<std::string> undoLastRegisteredOperations(const int undoCount);

private:
    // LIFO stack to keep order of the operations by keeping track of the operands of each
    // expression.
    std::stack<std::string> mOperandOrderStack;

    // Map holding the operands with already available values.
    std::unordered_map<std::string, int> mOperandValuesMap;

    // Map used to keep track of the dependencies between operands. This is a one to many
    // association since they key is the dependency and the value is a list of operands that
    // depend on the operand defined by the key.
    std::unordered_multimap<std::string, std::string> mOperandDependenciesMap;

    // Map used to keep track of arithmetic expressions that depend on the value of other
    // operands.
    std::unordered_map<std::string, Parser> mExpressionsWithDependenciesMap;
};

} // namespace Calculator