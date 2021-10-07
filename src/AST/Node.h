#pragma once

#include <iostream>

namespace AST {

class Node
{
public:
    explicit Node(char nodeValue,
                  std::shared_ptr<Node> leftNode = nullptr,
                  std::shared_ptr<Node> rightNode = nullptr)
        : mNodeValue{nodeValue}
        , mLeftNode{std::move(leftNode)}
        , mRightNode{std::move(rightNode)}
    {
    }

    [[nodiscard]] char getNodeValue() const
    {
        return mNodeValue;
    }

    [[nodiscard]] std::shared_ptr<Node> getLeftNode() const
    {
        return mLeftNode;
    }

    [[nodiscard]] std::shared_ptr<Node> getRightNode() const
    {
        return mRightNode;
    }

    void printNode()
    {
        printNodeInternal(this);
    }

private:
    void printNodeInternal(const Node* node, std::string&& prefix = "")
    {
        if (node) {
            // print the value of the node
            std::cout << prefix << node->getNodeValue() << "\n";

            // enter the next tree level - left and right branch
            printNodeInternal(node->getLeftNode().get(), prefix + "    ");
            printNodeInternal(node->getRightNode().get(), prefix + "    ");
        }
    }

private:
    char mNodeValue{};
    std::shared_ptr<Node> mLeftNode{nullptr};
    std::shared_ptr<Node> mRightNode{nullptr};
};

} // namespace AST
