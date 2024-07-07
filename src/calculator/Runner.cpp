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
    OTHER = 2   // Most probably an arithmetic expression (needs further evaluation).
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
    // Handle situations where the user provided a supported instructions
    // instead of an arithmetic expression.
    {
        const auto operationRequest = getOperationRequest(input);

        switch (operationRequest.first) {
        case SupportedOperation::RESULT: {
            const auto lastOperation = mState.getLastFulfilledOperation();

            if (lastOperation == decltype(lastOperation)()) {
                std::cout << "There is no result available yet\n";
            } else {
                std::cout << "return " << lastOperation.first << " = " << lastOperation.second
                          << "\n";
            }
            return;
        }
        case SupportedOperation::UNDO: {
            const auto undoneOperations = mState.undoLastRegisteredOperations(
                  operationRequest.second.value_or(/*default*/ 0));

            if (undoneOperations.empty()) {
                std::cout << "No operations were undone\n";
                return;
            }

            for (const auto& undoneOperation : undoneOperations) {
                std::cout << "delete " << undoneOperation << "\n";
            }

            return;
        }
        case SupportedOperation::OTHER:
        default:
            break;
        }
    }

    // Try to parse the provided arithmetic expression
    Parser expressionParser(input);
    if (!expressionParser.execute()) {
        std::cout << "\nInvalid arithmetic expression provided.";
        return;
    }

    // Retrieve the LHS of the parsed arithmetic expression (the operand).
    const auto expressionOperand = expressionParser.getOperandOfLHS();
    // Retrieve the RHS of the parsed arithmetic expression (the AST).
    const auto& expressionAST = expressionParser.getASTOfRHS();

    // Try to evaluate the AST to check if we can obtain
    // either a valid result
    // or a list of unmet dependencies.
    Evaluator astEvaluator(expressionAST,
                           /* map with the current values of each operand
                              is used as a lookup table when evaluation the AST.  */
                           mState.getOperandValueMap());

    const auto evaluationResult = astEvaluator.execute();

    std::visit(
          [&](auto&& variantValue) {
              using VariantType = std::decay_t<decltype(variantValue)>;

              if constexpr (std::is_same_v<VariantType, int>) {

                  for (const auto& [operand, value] :
                       mState.storeExpressionValue(expressionOperand, variantValue)) {

                      std::cout << operand << " = " << value << "\n";
                  }

                  mState.updateOperationOrder(expressionOperand);

              } else if constexpr (std::is_same_v<VariantType, std::unordered_set<std::string>>) {

                  if (!variantValue.empty()) {

                      // Update dependencies since we didn't get a valid value after the evaluation.
                      if (mState.storeExpressionDependencies(
                                expressionOperand, std::move(expressionParser), variantValue)) {
                          mState.updateOperationOrder(expressionOperand);
                      }
                  }
              }
          },
          evaluationResult);
}

} // namespace Calculator