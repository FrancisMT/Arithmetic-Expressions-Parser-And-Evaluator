#include "Parser.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

#include "ast/Node.h"
#include "mathUtils/Constants.hpp"

#include <unordered_set>

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

std::vector<std::string> splitString(const std::string& stringToSplit, const char splitChar)
{
    std::vector<std::string> output;
    for (const auto& splitValue : stringToSplit | std::ranges::views::split(splitChar)) {
        output.emplace_back(splitValue.begin(), splitValue.end());
    }

    return output;
}

void removeWhiteSpacesFromString(std::string& stringToTrim)
{
    stringToTrim.erase(
          std::remove_if(stringToTrim.begin(),
                         stringToTrim.end(),
                         [](unsigned char stringChar) { return std::isspace(stringChar); }),
          stringToTrim.end());
}

} // namespace

Parser::Parser(const std::string& inputToParse)
    : mInputString{inputToParse}
{
}

bool Parser::execute()
{
    removeWhiteSpacesFromString(mInputString);

    const auto inputStringTokens = splitString(mInputString, MathUtils::Constants::cAssignOp);

#ifdef DEBUG_BUILD
    std::cout << "\nInput String tokens:\n";
    for (const auto& stringToken : inputStringTokens) {
        std::cout << stringToken << "\n";
    }
#endif

    if (inputStringTokens.size() != 2) {
        std::cout << "Invalid arithmetic operation provided.";
        return false;
    }

    mLHSString = inputStringTokens.front();
    mRHSString = inputStringTokens.back();

    return parseLHS() && parseRHS();
}

[[nodiscard]] std::string Parser::getOperandOfLHS() const
{
    return mLHSString;
}

const std::unique_ptr<AST::Node>& Parser::getASTOfRHS() const
{
    return mRHSValueStack.top();
}

bool Parser::parseLHS()
{
    // TODO[FM]: Add support for a more complex parsing.
    // Ideally we would also create an AST for the LHS but for now,
    // we only support LHS values with a single letter operands (e.g. "x=2+2").
    if (mLHSString.size() != 1) {
        return false;
    }

    return true;
}

bool Parser::parseRHS()
{
    return validateRHS() && createASTforRHS();
}

bool Parser::validateRHS()
{
    std::cout << "Validating input string: " << mRHSString << "\n";

    if (mRHSString.empty()) {
        std::cout << "Empty expression provided" << "\n";
        return false;
    }

    // String will be wrapped around parenthesis for easier parsing
    std::string validatedString{cLeftParenthesis};
    validatedString.reserve(mRHSString.size());

    uint8_t leftParenthesisCounter{0};
    uint8_t rightParenthesisCounter{0};

    for (const auto& character : mRHSString) {

        const auto& previousValidCharacter = validatedString.back();

        // // Trim input string
        // if (character == cSpaceChar) {
        //     continue;
        // }
        // Check digits validity
        /* else */ if (isSingleDigitInteger(previousValidCharacter, character)) {

            // Right parenthesis should not be followed by a digit
            if (previousValidCharacter == cRightParenthesis) {
                std::cout << "Invalid expression provided" << "\n";
                return false;
            }
        }
        // Check operators validity
        else if (isOperator(character)) {

            // Check if the operator is used as a unary minus
            if (isUnaryMinus(previousValidCharacter, character)) {
                std::cout << "Negative values are not currently supported" << "\n";
                return false;
            }

            // Check if the operator syntax is correct
            if (isOperator(previousValidCharacter) || previousValidCharacter == cLeftParenthesis) {
                std::cout << "Invalid expression provided" << "\n";
                return false;
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
                std::cout << "Invalid expression provided" << "\n";
                return false;
            }
        }
        // Account for single character variables
        else if (!std::isalpha(character)) {
            std::cout << "Invalid expression provided" << "\n";
            return false;
        }

        validatedString.push_back(character);
    }

    // Validate the amount of parenthesis pairs
    if (leftParenthesisCounter != rightParenthesisCounter) {
        std::cout << "Parenthesis do not match" << "\n";
        return false;
    }

    // Validate that the expression does not end with an operator
    if (isOperator(validatedString.back())) {
        std::cout << "Invalid expression provided" << "\n";
        return false;
    }

    validatedString.append(1, cRightParenthesis);
    mRHSString.swap(validatedString);
    mRHSString.shrink_to_fit();
    std::cout << "Validated String: " << mRHSString << "\n";

    return true;
}

bool Parser::createASTforRHS()
{
    std::cout << "Generating Abstract Syntax Tree\n";

    const auto generateNewNode = [this]() {
        if (!mRHSOperatorStack.empty() && !mRHSValueStack.empty()) {

            const auto operation = mRHSOperatorStack.top();
            mRHSOperatorStack.pop();

            auto rightValue = std::move(mRHSValueStack.top());
            mRHSValueStack.pop();

            auto leftValue = std::move(mRHSValueStack.top());
            mRHSValueStack.pop();

            mRHSValueStack.emplace(std::make_unique<AST::Node>(
                  operation, std::move(leftValue), std::move(rightValue)));
        }
    };

    // Analyse input
    for (const auto& character : mRHSString) {

        // Account for the possibility that we might have a variable in the provided string.
        if (std::isdigit(character) || std::isalpha(character)) {
            mRHSValueStack.emplace(std::make_unique<AST::Node>(character));

        } else if (isOperator(character)) {
            while (!mRHSOperatorStack.empty()
                   && operatorPrecedence(mRHSOperatorStack.top())
                            >= operatorPrecedence(character)) {
                generateNewNode();
            }

            mRHSOperatorStack.push(character);

        } else if (character == cLeftParenthesis) {
            mRHSOperatorStack.push(character);

        } else if (character == cRightParenthesis) {
            while (!mRHSOperatorStack.empty() && mRHSOperatorStack.top() != cLeftParenthesis) {
                generateNewNode();
            }

            // Pop left parenthesis
            if (!mRHSOperatorStack.empty()) {
                mRHSOperatorStack.pop();
            }
        }
    }

    // Generates new nodes until the operator stack is empty
    while (!mRHSOperatorStack.empty()) {
        generateNewNode();
    }

#ifdef DEBUG_BUILD
    if (!mRHSValueStack.empty()) {
        std::cout << "Generated Abstract Syntax Tree\n";
        AST::printAST(mRHSValueStack.top());
    }
#endif

    return true;
}
