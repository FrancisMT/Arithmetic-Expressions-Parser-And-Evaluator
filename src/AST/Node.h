#pragma once

#include <iostream>

namespace AST {

/**
 * @brief Implementation of an AST Node
 */
class Node
{
public:
    /**
     * @brief Class constructor
     *
     * @param nodeValue char representing the value that the node will hold
     * @param leftNode Left child node
     * @param rightNode Right child node
     */
    explicit Node(char nodeValue,
                  std::shared_ptr<Node> leftNode = nullptr,
                  std::shared_ptr<Node> rightNode = nullptr)
        : mNodeValue{nodeValue}
        , mLeftNode{std::move(leftNode)}
        , mRightNode{std::move(rightNode)}
    {
    }

    /**
     * @brief Getter for the value currently being held by the node
     *
     * @return value being held by the node
     */
    [[nodiscard]] char getNodeValue() const
    {
        return mNodeValue;
    }

    /**
     * @brief Getter for the left child node
     *
     * @return shared pointer to the left child node
     */
    [[nodiscard]] std::shared_ptr<Node> getLeftNode() const
    {
        return mLeftNode;
    }

    /**
     * @brief Getter for the right child node
     *
     * @return shared pointer to the right child node
     */
    [[nodiscard]] std::shared_ptr<Node> getRightNode() const
    {
        return mRightNode;
    }

    /**
     * @brief Helper method used to print the contents of the node horizontally
     */
    void printNode()
    {
        printNodeInternal(this);
    }

private:
    /**
     * @brief Method used for recursively print the node's contents
     *
     * Recursive calls are made in order to print the content of every child node's value
     *
     * @param node Node whose content will be accessed and printed out
     * @param prefix helper string used to beautify the outputted data
     */
    void printNodeInternal(const Node* node, std::string&& prefix = "")
    {
        if (node) {
            std::cout << prefix << node->getNodeValue() << "\n";
            printNodeInternal(node->getLeftNode().get(), prefix + "    ");
            printNodeInternal(node->getRightNode().get(), prefix + "    ");
        }
    }

private:
    /// Value being held by the node
    char mNodeValue{};
    /// Left child node
    std::shared_ptr<Node> mLeftNode{nullptr};
    /// Right child node
    std::shared_ptr<Node> mRightNode{nullptr};
};
} // namespace AST
