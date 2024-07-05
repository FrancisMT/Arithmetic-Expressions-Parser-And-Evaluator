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
                  std::unique_ptr<Node> leftNode = nullptr,
                  std::unique_ptr<Node> rightNode = nullptr)
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
     * @return reference to the left child node pointer
     */
    [[nodiscard]] const std::unique_ptr<Node>& getReferenceToLeftNodePointer() const
    {
        return mLeftNode;
    }

    /**
     * @brief Getter for the right child node
     *
     * @return reference to the right child node pointer
     */
    [[nodiscard]] const std::unique_ptr<Node>& getReferenceToRightNodePointer() const
    {
        return mRightNode;
    }

private:
    /// Value being held by the node
    char mNodeValue{};
    /// Left child node
    std::unique_ptr<Node> mLeftNode{nullptr};
    /// Right child node
    std::unique_ptr<Node> mRightNode{nullptr};
};

/**
 * @brief Helper method used to count the total number of nodes inside an AST
 *
 * @param rootNode Reference to the root node of the AST
 *
 * @return Number of nodes inside the AST
 */
[[nodiscard]] inline uint32_t getNumberOfNodes(const std::unique_ptr<Node>& rootNode)
{
    return !rootNode ? 0
                     : 1 + getNumberOfNodes(rootNode->getReferenceToLeftNodePointer())
                             + getNumberOfNodes(rootNode->getReferenceToRightNodePointer());
}

/**
 * @brief Helper method used to print (horizontally) the contents of an AST
 *
 * Recursive calls are made in order to print the content of every node's value using preorder
 * traversal
 *
 * @param rootNode Reference to the root node of the AST
 * @param prefix helper string used to beautify the outputted data
 */
inline void printAST(const std::unique_ptr<Node>& rootNode, std::string&& prefix = "")
{
    if (rootNode) {
        std::cout << prefix << rootNode->getNodeValue() << "\n";
        printAST(rootNode->getReferenceToLeftNodePointer(), prefix + "    ");
        printAST(rootNode->getReferenceToRightNodePointer(), prefix + "    ");
    }
}

/**
 * @brief Helper method used to compare two ASTs
 *
 * @param rootNodeA Reference to the root node of the first AST
 * @param rootNodeB Reference to the root node of the second AST
 *
 * @return Boolean containing the comparison result
 */
[[nodiscard]] inline bool areASTsIdentical(const std::unique_ptr<Node>& rootNodeA,
                                           const std::unique_ptr<Node>& rootNodeB)
{
    return (!rootNodeA && !rootNodeB)
           || ((rootNodeA && rootNodeB) && (rootNodeA->getNodeValue() == rootNodeB->getNodeValue())
               && areASTsIdentical(rootNodeA->getReferenceToLeftNodePointer(),
                                   rootNodeB->getReferenceToLeftNodePointer())
               && areASTsIdentical(rootNodeA->getReferenceToRightNodePointer(),
                                   rootNodeB->getReferenceToRightNodePointer()));
}

} // namespace AST
