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

private:
    /// Value being held by the node
    char mNodeValue{};
    /// Left child node
    std::shared_ptr<Node> mLeftNode{nullptr};
    /// Right child node
    std::shared_ptr<Node> mRightNode{nullptr};
};

/**
 * @brief Helper method used to count total number of nodes inside an AST
 *
 * @param rootNode Root node of the AST
 *
 * @return Number of nodes inside the AST
 */
[[nodiscard]] inline uint32_t getNumberOfNodes(const Node* rootNode)
{
    return !rootNode ? 0
                     : 1 + getNumberOfNodes(rootNode->getLeftNode().get())
                             + getNumberOfNodes(rootNode->getRightNode().get());
}

/**
 * @brief Helper method used to print (horizontally) the contents of an AST
 *
 * Recursive calls are made in order to print the content of every child rootNode's value
 *
 * @param rootNode Root node of the AST
 * @param prefix helper string used to beautify the outputted data
 */
inline void printAST(const Node* rootNode, std::string&& prefix = "")
{
    if (rootNode) {
        std::cout << prefix << rootNode->getNodeValue() << "\n";
        printAST(rootNode->getLeftNode().get(), prefix + "    ");
        printAST(rootNode->getRightNode().get(), prefix + "    ");
    }
}

/**
 * @brief Helper method used to compare two ASTs
 *
 * @param rootNodeA Root node of the first AST
 * @param rootNodeB Root node of the second AST
 *
 * @return Boolean containing the comparison result
 */
[[nodiscard]] inline bool areASTsIdentical(const Node* rootNodeA, const Node* rootNodeB)
{
    return (!rootNodeA && !rootNodeB)
           || ((rootNodeA && rootNodeB) && (rootNodeA->getNodeValue() == rootNodeB->getNodeValue())
               && areASTsIdentical(rootNodeA->getLeftNode().get(), rootNodeB->getLeftNode().get())
               && areASTsIdentical(rootNodeA->getRightNode().get(),
                                   rootNodeB->getRightNode().get()));
}

} // namespace AST
