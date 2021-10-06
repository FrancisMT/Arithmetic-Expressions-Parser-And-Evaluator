#include "Parser.h"

#include <cctype>
#include <iostream>

#include "AST/Node.h"

#include <unordered_set>

namespace {
constexpr auto cSpaceChar{' '};
constexpr auto cLeftParenthesis{'('};
constexpr auto cRightParenthesis{')'};
constexpr auto cSumOp{'+'};
constexpr auto cSubOp{'-'};
constexpr auto cMultOp{'*'};
constexpr auto cDivOp{'/'};
const std::unordered_set<char> cValidParentheses{cLeftParenthesis, cRightParenthesis};
const std::unordered_set<char> cValidOperators{cSumOp, cSubOp, cMultOp, cDivOp};

bool isParenthesis(const char character)
{
    return cValidParentheses.count(character);
}

bool isOperator(const char character)
{
    return cValidOperators.count(character);
}

bool isSingleDigitInteger(const char character, const char nextCharacter)
{
    return std::isdigit(character) && (!std::isdigit(nextCharacter) || isOperator(nextCharacter));
}

bool isUnaryMinus(const char previousCharacter, const char character)
{
    return character == cSubOp
           && (isOperator(previousCharacter) || previousCharacter == cSpaceChar
               || previousCharacter == cLeftParenthesis);
}

uint8_t getOperatorPrecedence(char operation)
{
    switch (operation) {
    case cRightParenthesis:
        return 1;
    case cMultOp:
    case cDivOp:
        return 2;
    case cSumOp:
    case cSubOp:
        return 3;
    case cLeftParenthesis:
        return 4;
    }
    return 0;
}

void printBT(const std::string& prefix, const Node* node, bool isLeft)
{
    if (node != nullptr) {
        std::cout << prefix;

        std::cout << (isLeft ? "├─left: " : "└──right: ");

        // print the value of the node
        std::cout << node->getNodeValue() << std::endl;

        // enter the next tree level - left and right branch
        printBT(prefix + (isLeft ? "│   " : "    "), node->getMLeftNode(), true);
        printBT(prefix + (isLeft ? "│   " : "    "), node->getMRightNode(), false);
    }
}

void printBT(const Node* node)
{
    printBT("", node, false);
}

} // namespace

Parser::Parser(std::string&& inputToParse)
    : mInputString{inputToParse}
{
}

void Parser::execute()
{
    validateInput();
    createAST();
}

void Parser::validateInput()
{
    std::cout << "Validating input string: " << mInputString << "\n";

    // String will be wrapped around parenthesis for easier parsing
    std::string validatedString{cLeftParenthesis};
    validatedString.reserve(mInputString.size());

    uint8_t leftParenthesisCounter{0};
    uint8_t rightParenthesisCounter{0};

    for (std::size_t stringIndex = 0; stringIndex < mInputString.size(); ++stringIndex) {

        const auto& character = mInputString[stringIndex];
        const auto& previousValidCharacter = validatedString.back();
        const auto& nextCharacter = (stringIndex == mInputString.size() - 1)
                                          ? cSpaceChar
                                          : mInputString[stringIndex + 1];

        // Trim input string
        if (character == cSpaceChar) {
            continue;
        }
        // Check digits validity
        else if (isSingleDigitInteger(character, nextCharacter)) {

            // Check parenthesis right bounds
            if (previousValidCharacter == cRightParenthesis) {
                throw std::invalid_argument("Invalid expression provided");
            }

            validatedString.push_back(character);
        }
        // Check operators validity
        else if (isOperator(character)) {

            // Check if the operator is used as a unary minus
            if (isUnaryMinus(previousValidCharacter, character)) {
                throw std::invalid_argument("Negative value provided");
            }

            // Check if the operator syntax is correct
            if (isOperator(previousValidCharacter) || previousValidCharacter == cLeftParenthesis
                || previousValidCharacter == cSpaceChar) {
                throw std::invalid_argument("Invalid expression provided");
            }

            validatedString.push_back(character);
        }
        // Check parenthesis validity
        else if (isParenthesis(character)) {

            // Keep tabs on the amount of parenthesis pairs
            if (character == cLeftParenthesis) {
                ++leftParenthesisCounter;
            } else if (character == cRightParenthesis) {
                ++rightParenthesisCounter;
            }

            // Check parenthesis left bounds
            if (character == cLeftParenthesis && (std::isdigit(previousValidCharacter))) {
                throw std::invalid_argument("Invalid expression provided");
            }

            validatedString.push_back(character);
        } else {
            throw std::invalid_argument("Invalid character used");
        }
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
    std::cout << "Creating AST\n";

    const auto processNode = [this]() {
        const auto operation = operatorStack.top();
        operatorStack.pop();

        const auto valueRight = valueStack.top();
        valueStack.pop();

        const auto valueLeft = valueStack.top();
        valueStack.pop();

        valueStack.push(new Node(operation, valueLeft, valueRight));
    };

    for (const auto& character : mInputString) {

        if (std::isdigit(character)) {
            valueStack.emplace(new Node(character));
        } else if (character == cLeftParenthesis) {
            operatorStack.push(character);
        } else if (character == cRightParenthesis) {
            while (!operatorStack.empty() && operatorStack.top() != cLeftParenthesis) {
                processNode();
            }

            // Pop left parenthesis
            if (!operatorStack.empty()) {
                operatorStack.pop();
            }
        } else if (isOperator(character)) {
            while (!operatorStack.empty()
                   && getOperatorPrecedence(character)
                            >= getOperatorPrecedence(operatorStack.top())) {
                processNode();
            }

            operatorStack.push(character);
        }
    }

    while (!operatorStack.empty()) {
        processNode();
    }

    printBT(valueStack.top());
}
