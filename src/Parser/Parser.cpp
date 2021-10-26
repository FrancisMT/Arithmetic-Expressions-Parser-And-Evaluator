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

/**
 * @brief Checks if the provided character is a parenthesis
 *
 * @param character character to evaluate
 *
 * @return Boolean containing the verification result
 */
constexpr bool isParenthesis(const char character)
{
    switch (character) {
    case cLeftParenthesis:
    case cRightParenthesis:
        return true;
    default:
        return false;
    }
}

/**
 * @brief Checks if the provided character is a valid binary operator
 *
 * @param character character to evaluate
 *
 * @return Boolean containing the verification result
 */
constexpr bool isOperator(const char character)
{
    switch (character) {
    case cAddOp:
    case cSubOp:
    case cMultOp:
    case cDivOp:
        return true;
    default:
        return false;
    }
}

/**
 * @brief Checks if the provided character is a single digit integer
 *
 * @param previousCharacter adjacent character to evaluate (can not be a digit)
 * @param character character to evaluate
 *
 * @return Boolean containing the verification result
 */
constexpr bool isSingleDigitInteger(const char previousCharacter, const char character)
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
constexpr bool isUnaryMinus(const char previousCharacter, const char character)
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
 * Opening parentheses act as operators with a very low precedence value
 * Closing parentheses act as operators with a very right precedence value
 *
 * @param operation operator to check the precedence of
 *
 * @return The operator's precedence value
 */
constexpr uint8_t operatorPrecedence(char operation)
{
    switch (operation) {
    case cRightParenthesis:
        return 4;
    case cMultOp:
    case cDivOp:
        return 3;
    case cAddOp:
    case cSubOp:
        return 2;
    case cLeftParenthesis:
        return 1;
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

const std::unique_ptr<AST::Node>& Parser::getAST() const
{
    return mValueStack.top();
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

            // Right parenthesis should not be followed by a digit
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
            if (character == cLeftParenthesis && std::isdigit(previousValidCharacter)) {
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

    // Validate that the expression does not end with an operator
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
        if (!mOperatorStack.empty() && !mValueStack.empty()) {

            const auto operation = mOperatorStack.top();
            mOperatorStack.pop();

            auto rightValue = std::move(mValueStack.top());
            mValueStack.pop();

            auto leftValue = std::move(mValueStack.top());
            mValueStack.pop();

            mValueStack.emplace(std::make_unique<AST::Node>(
                  operation, std::move(leftValue), std::move(rightValue)));
        }
    };

    // Analyse input
    for (const auto& character : mInputString) {

        if (std::isdigit(character)) {
            mValueStack.emplace(std::make_unique<AST::Node>(character));

        } else if (isOperator(character)) {
            while (!mOperatorStack.empty()
                   && operatorPrecedence(mOperatorStack.top()) >= operatorPrecedence(character)) {
                generateNewNode();
            }

            mOperatorStack.push(character);

        } else if (character == cLeftParenthesis) {
            mOperatorStack.push(character);


        } else if (character == cRightParenthesis) {
            while (!mOperatorStack.empty() && mOperatorStack.top() != cLeftParenthesis) {
                generateNewNode();
            }

            // Pop left parenthesis
            if (!mOperatorStack.empty()) {
                mOperatorStack.pop();
            }
        }
    }

    // Generates new nodes until the operator stack is empty
    while (!mOperatorStack.empty()) {
        generateNewNode();
    }

#ifdef DEBUG_BUILD
    if (!mValueStack.empty()) {
        std::cout << "Generated Abstract Syntax Tree\n";
        AST::printAST(mValueStack.top());
    }
#endif
}
