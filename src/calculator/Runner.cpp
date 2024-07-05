#include "Runner.hpp"

#include "evaluator/Evaluator.hpp"
#include "mathUtils/Constants.hpp"
#include "mathUtils/Methods.hpp"
#include "parser/Parser.hpp"

namespace {
/// Supported string for the undo command.
constexpr auto cUndoCommand{"undo"};
/// Supported string for the result command.
constexpr auto cResultCommand{"result"};

enum class SupportedOperation : uint8_t {

    RESULT = 0, // Present result of last fulfilled operation
    UNDO = 1,   // Undo a certain amount of operation
    OTHER = 2   // Most probably an arithmetic expression.
};

std::pair<SupportedOperation, std::optional<int>> getOperationRequest(const std::string& input)
{
    const auto inputStringTokens
          = MathUtils::Methods::splitString(input, MathUtils::Constants::cWhiteSpace);

    if (inputStringTokens.size() == 1 && inputStringTokens.back() == cResultCommand) {
        return {SupportedOperation::RESULT, {}};
    } else if (inputStringTokens.size() == 2 && inputStringTokens.front() == cUndoCommand) {

        const auto undoOperationCount = std::stoi(inputStringTokens.back());

        return {SupportedOperation::UNDO, undoOperationCount};
    }

    return {SupportedOperation::OTHER, {}};
}

} // namespace

namespace Calculator {

Runner::Runner() {}

Runner::~Runner() {}

void Runner::processInstruction(const std::string& input)
{
    // Check if the user provided a supported instruction (instead of an arithmetic expression).
    const auto operationRequest = getOperationRequest(input);

    switch (operationRequest.first) {
    case SupportedOperation::RESULT: {
        const auto lastOperation = mState.getLastFulfilledOperation();

        std::cout << "return " << lastOperation.first << " = " << lastOperation.second;
        return;
    }
    case SupportedOperation::UNDO: {
        const auto undoneOperations
              = mState.undoLastRegisteredOperations(operationRequest.second.value_or(0));
        return;
    }
    case SupportedOperation::OTHER:
    default:
        break;
    }

    {
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

            for (int stackPoperCount = 0; stackPoperCount < undoOperationCount; ++stackPoperCount) {

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

                AST::printAST(expressionsWithDependenciesMap.at(dependantOperand).getASTOfRHS());
                Evaluator evaluator(
                      expressionsWithDependenciesMap.at(dependantOperand).getASTOfRHS(),
                      operandValuesMap);
                const auto evaluatorResult = evaluator.execute();
                if (evaluatorResult.has_value()) {
                    operandValuesMap.insert_or_assign(dependantOperand, evaluatorResult.value());
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

} // namespace Calculator