#include "Evaluator.h"

#include "MathUtils/Constants.h"

Evaluator::Evaluator(std::shared_ptr<AST::Node> astRootNode)
    : mAstRootNode{std::move(astRootNode)}
{
}

int32_t Evaluator::execute()
{
    if (!mAstRootNode) {
        throw std::invalid_argument("Empty AST");
    }

    return analyseAndTraverseASTNode(mAstRootNode);
}

int32_t Evaluator::analyseAndTraverseASTNode(const std::shared_ptr<AST::Node>& node)
{
    const auto nodeValue = node->getNodeValue();

    if (std::isdigit(nodeValue)) {
        return static_cast<int32_t>(nodeValue - '0');
    } else {
        const auto leftNodeValue = analyseAndTraverseASTNode(node->getLeftNode());
        const auto rightNodeValue = analyseAndTraverseASTNode(node->getRightNode());

        using namespace MathUtils::Constants;
        switch (nodeValue) {
        case cAddOp:
            return leftNodeValue + rightNodeValue;
        case cSubOp:
            return leftNodeValue - rightNodeValue;
        case cMultOp:
            return leftNodeValue * rightNodeValue;
        case cDivOp:
            return leftNodeValue / rightNodeValue;
        default:
            return 1;
        }
    }
}
