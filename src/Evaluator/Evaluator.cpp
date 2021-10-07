#include "Evaluator.h"

#include "MathUtils/MathConstants.h"

Evaluator::Evaluator(std::shared_ptr<AST::Node> astRootNode)
    : mAstRootNode{std::move(astRootNode)}
{
}

int32_t Evaluator::execute()
{
    if (!mAstRootNode) {
        throw std::invalid_argument("Empty AST");
    }

    return traverseAST(mAstRootNode);
}

int32_t Evaluator::traverseAST(const std::shared_ptr<AST::Node>& node)
{
    const auto nodeValue = node->getNodeValue();

    if (std::isdigit(nodeValue)) {
        return static_cast<int32_t>(nodeValue - '0');
    } else {
        const auto leftNodeValue = traverseAST(node->getLeftNode());
        const auto rightNodeValue = traverseAST(node->getRightNode());

        using namespace MathUtils;
        switch (nodeValue) {
        case cSumOp:
            return leftNodeValue + rightNodeValue;
        case cSubOp:
            return leftNodeValue - rightNodeValue;
        case cMultOp:
            return leftNodeValue * rightNodeValue;
        case cDivOp:
            return leftNodeValue / rightNodeValue;
        }
    }

    return 0;
}
