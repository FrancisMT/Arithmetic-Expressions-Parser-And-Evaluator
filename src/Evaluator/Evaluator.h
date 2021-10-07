#pragma once

#include <memory>

#include "AST/Node.h"

/**
 * @brief Class responsible for evaluating arithmetic expressions contained in an AST
 */
class Evaluator
{
public:
    /**
     * @brief Class constructor
     *
     * @param astRootNode Root node of an Abstract Syntax Tree (AST)
     */
    explicit Evaluator(std::shared_ptr<AST::Node> astRootNode);

    /**
     * @brief Evaluates an arithmetic expression held by the AST and outputs a result
     *
     * @return Result of the arithmetic expression
     */
    int32_t execute();

private:
    /**
     * @brief Helper method used to recursively traverse the AST and evaluate each node's content
     *
     * @param node AST node to analyse
     *
     * @return Final value of the node
     */
    int32_t analyseAndTraverseASTNode(const std::shared_ptr<AST::Node>& node);

private:
    /// Abstract syntax tree root node
    std::shared_ptr<AST::Node> mAstRootNode;
};
