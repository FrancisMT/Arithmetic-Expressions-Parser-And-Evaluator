#pragma once

#include <memory>
#include <stack>
#include <string>

#include "AST/Node.h"

/**
 * @brief Class responsible for parsing arithmetic expressions into an AST
 */
class Parser
{
public:
    /**
     * @brief Class constructor
     *
     * @param inputToParse String containing data to be parsed
     */
    explicit Parser(const std::string& inputToParse);

    /**
     * @brief Checks the input for a valid arithmetic expression and generates the appropriate AST
     */
    void execute();

    /**
     * @brief Getter for the generated AST
     *
     * @return Reference to the root node of the generated AST
     */
    [[nodiscard]] const std::unique_ptr<AST::Node>& getAST() const;

private:
    /**
     * @brief Helper method used to assert if the input string is a valid arithmetic expression
     */
    void validateInput();

    /**
     * @brief Helper method used to parse an arithmetic expression into an AST
     *
     * The AST is created using the Shunting Yard algorithm
     */
    void createAST();

private:
    /// String containing the input of the parser
    std::string mInputString;
    /// LIFO container used to handle the operators of an arithmetic expression
    std::stack<char> mOperatorStack;
    /// LIFO container used to handle the operands of an arithmetic expression
    std::stack<std::unique_ptr<AST::Node>> mValueStack;
};
