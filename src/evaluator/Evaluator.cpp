#include "Evaluator.hpp"

#include "mathUtils/Constants.hpp"

namespace {
/**
 * @brief Performs an arithmetic operation
 *
 * @param operation Binary operation type
 * @param leftOperand Left Operand
 * @param rightOperand Right Operand
 *
 * @return Operation result
 */
constexpr auto performArithmeticOperation(char operation, float leftOperand, float rightOperand)
{
    using namespace MathUtils::Constants;
    switch (operation) {
    case cAddOp:
        return leftOperand + rightOperand;
    case cSubOp:
        return leftOperand - rightOperand;
    case cMultOp:
        return leftOperand * rightOperand;
    case cDivOp:
        return leftOperand / rightOperand;
    default:
        return 1.f;
    }
}

constexpr bool isOperator(const char character)
{
    using namespace MathUtils::Constants;

    switch (character) {
    case cAddOp:
    case cSubOp:
    case cMultOp:
    case cDivOp:
        return true;
    default:
        return false;
    }
}
} // namespace

Evaluator::Evaluator(const std::unique_ptr<AST::Node>& astRootNode,
                     const std::unordered_map<std::string, int>& operandLookupMap)
    : mAstRootNode{astRootNode}
    , mOperandLookupMap{operandLookupMap}
{
}

Evaluator::Result Evaluator::execute()
{
    if (!mAstRootNode) {
        std::cout << "Empty AST";
        return {};
    }

    const auto expressionValue = static_cast<int32_t>(analyseAndTraverseASTNode(mAstRootNode));

    if (!mDependencies.empty()) {
        return mDependencies;
    }

    return expressionValue;
}

float Evaluator::analyseAndTraverseASTNode(const std::unique_ptr<AST::Node>& node)
{
    const auto nodeValue = node->getNodeValue();

    std::unordered_set<char> variableDependencies;

    if (std::isdigit(nodeValue)) {
        return static_cast<float>(nodeValue - '0');
    }
    // Is a variable
    else if (std::isalpha(nodeValue)) {

        // Check if the variable exists in the results map and if so, return the corresponding
        // value. Otherwise, add it to the dependencies list.

        const auto nodeValueString = {nodeValue};

        if (mOperandLookupMap.contains(nodeValueString)) {
            return static_cast<float>(mOperandLookupMap.at(nodeValueString));
        }

        mDependencies.insert({nodeValue});

        // TODO: I need to continue to traverse the AST!!!!!!!!
    } else {
        const auto leftNodeValue = analyseAndTraverseASTNode(node->getReferenceToLeftNodePointer());
        const auto rightNodeValue
              = analyseAndTraverseASTNode(node->getReferenceToRightNodePointer());

        return performArithmeticOperation(nodeValue, leftNodeValue, rightNodeValue);
    }

    return 0.f;
}
