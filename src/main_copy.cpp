#include <iostream>
#include <string>

#include "evaluator/Evaluator.hpp"
#include "mathUtils/Methods.hpp"
#include "parser/Parser.hpp"

// For the calculator state prototype
#include <stack>

#include <unordered_map>
#include <unordered_set>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Calculator state prototype

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

    while (true) {

        // Get input from user
        std::cout << "\nInput Arithmetic expression to evaluate: ";
        std::string input;
        std::getline(std::cin, input);

        // Check if the user provided a supported set of instructions instead of an arithmetic
        // expression.
        {
            constexpr auto cUndoCommand{"undo"};
            constexpr auto cResultCommand{"result"};

            const auto inputStringTokens = MathUtils::Methods::splitString(input, ' ');

            if (inputStringTokens.size() == 1 && inputStringTokens.back() == cResultCommand) {
                std::cout << "Result command detected: return the result of the last requested "
                             "operation that has already been fulfilled\n";

                auto operandOrderStackTemp = operandOrderStack;

                // Go through the stack of operations history and check if the corresponding operand
                // already has a value available.
                while (!operandOrderStackTemp.empty()) {

                    if (operandValuesMap.contains(operandOrderStackTemp.top())) {

                        std::cout << "return " << operandOrderStackTemp.top() << " = "
                                  << operandValuesMap.at(operandOrderStackTemp.top());
                        break;
                    }

                    operandOrderStackTemp.pop();
                }

                continue;

            } else if (inputStringTokens.size() == 2 && inputStringTokens.front() == cUndoCommand) {

                const auto undoOperationCount = std::stoi(inputStringTokens.back());
                std::cout << "Undo command detected: undoing the last " << undoOperationCount
                          << "operations";

                if (undoOperationCount <= 0) {

                    std::cout << "No operations will be undone";
                    continue;
                }

                for (int stackPoperCount = 0; stackPoperCount < undoOperationCount;
                     ++stackPoperCount) {

                    // We delete the necessary value from the operations history stack.
                    const auto operand = operandOrderStack.top();
                    operandOrderStack.pop();

                    // We check if the we need to either delete the entry in the operandValuesMap or
                    // the expressionsWithDependenciesMap
                    if (operandValuesMap.contains(operand)) {
                        operandValuesMap.erase(operand);
                    }

                    if (expressionsWithDependenciesMap.contains(operand)) {
                        expressionsWithDependenciesMap.erase(operand);
                    }

                    std::cout << "delete " << operand;
                }

                continue;
            }
        }

        // std::string inputA = "   a    =    b    *   ( 3 +2/a)   ";
        Parser inputParser(input);

        const auto res = inputParser.execute();
        std::cout << std::boolalpha << res;

        if (!res) {
            std::cout << "\nPlease provid a valid expression.";
            continue;
        }

        const auto operand = inputParser.getOperandOfLHS();
        const auto& operationAST = inputParser.getASTOfRHS();

        Evaluator astEvaluator(operationAST, operandValuesMap);
        const auto optionalResult = astEvaluator.execute();

        const auto& resultDependencies = astEvaluator.getASTDependencies();

        // If we get a value and no dependencies, we can safelly update the operandValuesMap
        if (optionalResult.has_value() && resultDependencies.empty()) {
            operandValuesMap.insert_or_assign(operand, optionalResult.value());

            // Everytime we get a new value from an expression, check if it resolves any pending
            // operation.
            const auto operandDependcyRange = operandDependenciesMap.equal_range(operand);
            for (auto itr = operandDependcyRange.first; itr != operandDependcyRange.second; ++itr) {

                std::cout << itr->first << ' ' << itr->second << '\n';

                const auto dependantOperand = itr->second;

                if (expressionsWithDependenciesMap.contains(dependantOperand)) {

                    AST::printAST(
                          expressionsWithDependenciesMap.at(dependantOperand).getASTOfRHS());
                    Evaluator evaluator(
                          expressionsWithDependenciesMap.at(dependantOperand).getASTOfRHS(),
                          operandValuesMap);
                    const auto evaluatorResult = evaluator.execute();
                    if (evaluatorResult.has_value()) {
                        operandValuesMap.insert_or_assign(dependantOperand,
                                                          evaluatorResult.value());
                    }
                }
            }
        }
        // Otherwise, update the expressionsWithDependenciesMap and operandDependenciesMap.
        else {

            expressionsWithDependenciesMap.insert_or_assign(operand, std::move(inputParser));

            for (const auto& dependency : resultDependencies) {
                operandDependenciesMap.emplace(dependency, operand);
            }
        }

        // Add operant of the current expression to the operandOrderStack (to keep the order)
        operandOrderStack.push(operand);

        // Current state logging
        {
            std::cout << "\n operandOrderStack\n";
            auto stack = operandOrderStack;
            while (!stack.empty()) {
                std::cout << stack.top() << std::endl;
                stack.pop();
            }

            std::cout << "\n operandValuesMap\n";
            for (const auto& pair : operandValuesMap) {
                std::cout << pair.first << ": " << pair.second << std::endl;
            }

            std::cout << "\n operandDependenciesMap\n";
            for (const auto& pair : operandDependenciesMap) {
                std::cout << pair.first << ": " << pair.second << std::endl;
            }

            std::cout << "\n expressionsWithDependenciesMap\n";
            for (const auto& pair : expressionsWithDependenciesMap) {
                std::cout << pair.first << ": \n";
                AST::printAST(pair.second.getASTOfRHS());
            }
        }
    }

    return 0;
}