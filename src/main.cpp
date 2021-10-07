#include <iostream>
#include <string>

#include "Evaluator/Evaluator.h"
#include "Parser/Parser.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Get input from user
    std::cout << "Input Arithmetic expression to evaluate: ";
    std::string input;
    std::getline(std::cin, input);

    // Parse input into an AST
    Parser inputParser(input);
    try {
        inputParser.execute();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error thrown during input parsing: " << e.what() << "\n";
        return 0;
    }

    // Evaluate AST and output result
    Evaluator astEvaluator(inputParser.getAST());
    try {
        std::cout << "Arithmetic Expression Result: " << astEvaluator.execute() << "\n";
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error thrown during AST evaluation: " << e.what() << "\n";
        return 0;
    }

    return 0;
}