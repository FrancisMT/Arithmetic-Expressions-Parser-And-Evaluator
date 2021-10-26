#pragma once

#include <memory>

#include "AST/Node.h"

/**
 * @brief Class responsible for evaluating arithmetic expressions contained in an AST
 *
 * Evaluation is only performed on integers
 */
class Evaluator
{
public:
    /**
     * @brief Class constructor
     *
     * @param astRootNode Reference to the toot node of an Abstract Syntax Tree (AST)
     */
    explicit Evaluator(const std::unique_ptr<AST::Node>& astRootNode);

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
     * @param node Reference to an AST node to analyse
     *
     * @return Final value of the node
     */
    float analyseAndTraverseASTNode(const std::unique_ptr<AST::Node>& node);

private:
    /// Reference to an Abstract syntax tree root node
    const std::unique_ptr<AST::Node>& mAstRootNode;
};
