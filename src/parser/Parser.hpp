#pragma once

#include <memory>
#include <stack>
#include <string>

#include "ast/Node.h"

/**
 * @brief Class responsible for parsing the right hand side of an arithmetic expression into an AST
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
    [[nodiscard]] bool execute();

    [[nodiscard]] std::string getOperandOfLHS() const;

    /**
     * @brief Getter for the generated AST
     *
     * @return Reference to the root node of the generated AST
     */
    [[nodiscard]] const std::unique_ptr<AST::Node>& getASTOfRHS() const;

private:
    [[nodiscard]] bool parseLHS();
    [[nodiscard]] bool parseRHS();

    /**
     * @brief Helper method used to assert if the input string is a valid arithmetic expression
     */
    bool validateRHS();

    /**
     * @brief Helper method used to parse an arithmetic expression into an AST
     *
     * The AST is created using the Shunting Yard algorithm
     */
    bool createASTforRHS();

private:
    std::string mLHSString;
    std::string mRHSString;

    /// String containing the input of the parser
    std::string mInputString;

    /// LIFO container used to handle the operators of the RHS arithmetic expression
    std::stack<char> mRHSOperatorStack;


    // TODO: Make this a struct.
    /// LIFO container used to handle the operands of the RHS arithmetic expression
    std::stack<std::unique_ptr<AST::Node>> mRHSValueStack;
};
