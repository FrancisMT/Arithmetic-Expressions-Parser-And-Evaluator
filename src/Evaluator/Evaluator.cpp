//
// Created by ctw00111 on 06/10/21.
//

#include "Evaluator.h"

namespace {

void printBT(const std::string& prefix, const Node* node, bool isLeft)
{
    if (node != nullptr) {
        std::cout << prefix;

        std::cout << (isLeft ? "├─left: " : "└──right: ");

        // print the value of the node
        std::cout << node->getNodeValue() << std::endl;

        // enter the next tree level - left and right branch
        printBT(prefix + (isLeft ? "│   " : "    "), node->getMLeftNode(), true);
        printBT(prefix + (isLeft ? "│   " : "    "), node->getMRightNode(), false);
    }
}

void printBT(const Node* node)
{
    printBT("", node, false);
}

}
Evaluator::Evaluator(std::shared_ptr<Node> astRootNode) {

    printBT(astRootNode.get());
}
