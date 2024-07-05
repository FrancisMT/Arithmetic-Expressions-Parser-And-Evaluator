
#include <iostream>
#include <string>

#include "calculator/Runner.hpp"

std::string getUserInputString()
{
    std::cout << "Input Arithmetic expression to evaluate: ";
    std::string input;
    std::getline(std::cin, input);

    return input;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{

    Calculator::Runner calculator;

    while (true) {
        calculator.processInstruction(getUserInputString());
    }

    return 0;
}