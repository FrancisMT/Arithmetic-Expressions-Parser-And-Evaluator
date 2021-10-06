#include <iostream>
#include <string>

#include "Parser/Parser.h"
#include "Evaluator/Evaluator.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Get input
    std::cout << "Input Arithmetic expression to evaluate: ";
    std::string input;
    std::getline(std::cin, input);

    // Parse
    Parser inputParser(std::move(input));
    try {
        inputParser.execute();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error thrown during input parsing: " << e.what() << "\n";
        return 0;
    }

    // Evaluate AST
    Evaluator astEvaluator(inputParser.getAST());

    return 0;
}