#pragma once

#include <memory>
#include <optional>

#include "ast/Node.h"

#include <unordered_map>
#include <unordered_set>

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
    explicit Evaluator(const std::unique_ptr<AST::Node>& astRootNode,
                       const std::unordered_map<std::string, int>& operandLookupMap);

    /**
     * @brief Evaluates an arithmetic expression held by the AST and outputs a result
     *
     * @param[out] dependencies
     *
     * @return Result of the arithmetic expression
     */
    [[nodiscard]] std::optional<uint32_t> execute();

    [[nodiscard]] const std::unordered_set<std::string>& getASTDependencies() const;

private:
    /**
     * @brief Helper method used to recursively traverse the AST and evaluate each node's content
     *
     * @param node Reference to an AST node to analyse
     *
     * @return Final value of the node
     */
    [[nodiscard]] float analyseAndTraverseASTNode(const std::unique_ptr<AST::Node>& node);

private:
    /// Reference to an Abstract syntax tree root node
    const std::unique_ptr<AST::Node>& mAstRootNode;

    const std::unordered_map<std::string, int>& mOperandLookupMap;
    std::unordered_set<std::string> mDependencies;
};
