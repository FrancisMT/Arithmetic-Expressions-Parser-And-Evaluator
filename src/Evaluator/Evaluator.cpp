#include "Evaluator.h"

#include "MathUtils/Constants.h"

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
} // namespace

Evaluator::Evaluator(std::shared_ptr<AST::Node> astRootNode)
    : mAstRootNode{std::move(astRootNode)}
{
}

int32_t Evaluator::execute()
{
    if (!mAstRootNode) {
        throw std::invalid_argument("Empty AST");
    }

    return static_cast<int32_t>(analyseAndTraverseASTNode(mAstRootNode));
}

float Evaluator::analyseAndTraverseASTNode(const std::shared_ptr<AST::Node>& node)
{
    const auto nodeValue = node->getNodeValue();

    if (std::isdigit(nodeValue)) {
        return static_cast<float>(nodeValue - '0');
    } else {
        const auto leftNodeValue = analyseAndTraverseASTNode(node->getLeftNode());
        const auto rightNodeValue = analyseAndTraverseASTNode(node->getRightNode());

        return performArithmeticOperation(nodeValue, leftNodeValue, rightNodeValue);
    }
}
