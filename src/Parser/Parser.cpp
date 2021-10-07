#include "Parser.h"

#include <cctype>
#include <iostream>
#include <utility>
#include <unordered_set>

#include "AST/Node.h"
#include "MathUtils/Constants.h"

namespace {
using namespace MathUtils::Constants;
/// Space character used to facilitate the parsing process
constexpr auto cSpaceChar{' '};
/// Container with the valid parenthesis used to facilitate the parsing process
const std::unordered_set cValidParentheses{cLeftParenthesis, cRightParenthesis};
/// Container with the valid operators used to facilitate the parsing process
const std::unordered_set cValidOperators{cAddOp, cSubOp, cMultOp, cDivOp};

/**
 * @brief Checks if the provided character is a parenthesis
 *
 * @param character character to evaluate
 *
 * @return Boolean containing the verification result
 */
bool isParenthesis(const char character)
{
    return cValidParentheses.count(character);
}

/**
 * @brief Checks if the provided character is a valid binary operator
 *
 * @param character character to evaluate
 *
 * @return Boolean containing the verification result
 */
bool isOperator(const char character)
{
    return cValidOperators.count(character);
}

/**
 * @brief Checks if the provided character is a single digit integer
 *
 * @param previousCharacter adjacent character to evaluate (can not be a digit)
 * @param character character to evaluate
 *
 * @return Boolean containing the verification result
 */
bool isSingleDigitInteger(const char previousCharacter, const char character)
{
    return !std::isdigit(previousCharacter) && std::isdigit(character);
}

/**
 * @brief Checks if the provided character is a unary minus
 *
 * @param previousCharacter adjacent character to evaluate (can not be an operator, space or '(')
 * @param character character to evaluate
 *
 * @return Boolean containing the verification result
 */
bool isUnaryMinus(const char previousCharacter, const char character)
{
    return character == cSubOp
           && (isOperator(previousCharacter) || previousCharacter == cSpaceChar
               || previousCharacter == cLeftParenthesis);
}

/**
 * @brief Checks precedence "score" of an operator
 *
 * This helper method is used during the construction of the AST: each operator being processed
 * causes its preceding operators to "execute" (new nodes in the AST are created) only if it has a
 * higher precedence value
 *
 * Opening parentheses act as operators with a very high precedence value
 * Closing parentheses act as operators with a very low precedence value
 *
 * @param operation operator to check the precedence of
 *
 * @return The operator's precedence value
 */
uint8_t operatorPrecedence(char operation)
{
    switch (operation) {
    case cRightParenthesis:
        return 1;
    case cMultOp:
    case cDivOp:
        return 2;
    case cAddOp:
    case cSubOp:
        return 3;
    case cLeftParenthesis:
        return 4;
    default:
        return 0;
    }
}
} // namespace

Parser::Parser(const std::string& inputToParse)
    : mInputString{inputToParse}
{
}

void Parser::execute()
{
    validateInput();
    createAST();
}

std::shared_ptr<AST::Node> Parser::getAST()
{
    auto rootNode = valueStack.top();
    return rootNode;
}

void Parser::validateInput()
{
    std::cout << "Validating input string: " << mInputString << "\n";

    if (mInputString.empty()) {
        throw std::invalid_argument("Empty expression provided");
    }

    // String will be wrapped around parenthesis for easier parsing
    std::string validatedString{cLeftParenthesis};
    validatedString.reserve(mInputString.size());

    uint8_t leftParenthesisCounter{0};
    uint8_t rightParenthesisCounter{0};

    for (const auto& character : mInputString) {

        const auto& previousValidCharacter = validatedString.back();

        // Trim input string
        if (character == cSpaceChar) {
            continue;
        }
        // Check digits validity
        else if (isSingleDigitInteger(previousValidCharacter, character)) {

            // Right parenthesis should not be follower by a digit
            if (previousValidCharacter == cRightParenthesis) {
                throw std::invalid_argument("Invalid expression provided");
            }
        }
        // Check operators validity
        else if (isOperator(character)) {

            // Check if the operator is used as a unary minus
            if (isUnaryMinus(previousValidCharacter, character)) {
                throw std::invalid_argument("Negative value provided");
            }

            // Check if the operator syntax is correct
            if (isOperator(previousValidCharacter) || previousValidCharacter == cLeftParenthesis) {
                throw std::invalid_argument("Invalid expression provided");
            }
        }
        // Check parenthesis validity
        else if (isParenthesis(character)) {

            // Keep tabs on the amount of parenthesis pairs
            if (character == cLeftParenthesis) {
                ++leftParenthesisCounter;
            } else if (character == cRightParenthesis) {
                ++rightParenthesisCounter;
            }

            // Left parenthesis should not be preceded by a digit
            if (character == cLeftParenthesis && (std::isdigit(previousValidCharacter))) {
                throw std::invalid_argument("Invalid expression provided");
            }
        } else {
            throw std::invalid_argument("Invalid character used");
        }

        validatedString.push_back(character);
    }

    // Validate the amount of parenthesis pairs
    if (leftParenthesisCounter != rightParenthesisCounter) {
        throw std::invalid_argument("Parenthesis do not match");
    }

    // Validate that the expression does not end with an operation
    if (isOperator(validatedString.back())) {
        throw std::invalid_argument("Invalid expression provided");
    }

    validatedString.append(1, cRightParenthesis);
    mInputString.swap(validatedString);
    mInputString.shrink_to_fit();
    std::cout << "Validated String: " << mInputString << "\n";
}

void Parser::createAST()
{
    std::cout << "Generating Abstract Syntax Tree\n";

    const auto generateNewNode = [this]() {
        if (!operatorStack.empty() && !valueStack.empty()) {

            const auto operation = operatorStack.top();
            operatorStack.pop();

            const auto rightValue = std::move(valueStack.top());
            valueStack.pop();

            const auto leftValue = std::move(valueStack.top());
            valueStack.pop();

            valueStack.emplace(std::make_shared<AST::Node>(operation, leftValue, rightValue));
        }
    };

    // Analyse input
    for (const auto& character : mInputString) {

        if (std::isdigit(character)) {
            valueStack.emplace(std::make_shared<AST::Node>(character));
        } else if (character == cLeftParenthesis) {
            operatorStack.push(character);
        } else if (character == cRightParenthesis) {
            while (!operatorStack.empty() && operatorStack.top() != cLeftParenthesis) {
                generateNewNode();
            }

            // Pop left parenthesis
            if (!operatorStack.empty()) {
                operatorStack.pop();
            }
        } else if (isOperator(character)) {
            while (!operatorStack.empty()
                   && operatorPrecedence(character) >= operatorPrecedence(operatorStack.top())) {
                generateNewNode();
            }

            operatorStack.push(character);
        }
    }

    // Generates new nodes until the operator stack is empty
    while (!operatorStack.empty()) {
        generateNewNode();
    }

    // Print AST for debugging purposes
    if (!valueStack.empty()) {
        std::cout << "Generated Abstract Syntax Tree\n";
        AST::printAST(valueStack.top().get());
    }
}
