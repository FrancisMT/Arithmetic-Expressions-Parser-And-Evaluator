
#include <iostream>
#include <string>

#include "calculator/Runner.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // const auto getUserInputString = [] {
    //     std::cout << "\nInput Arithmetic expression to evaluate: ";
    //     std::string input;
    //     std::getline(std::cin, input);

    //     return input;
    // };

    Calculator::Runner calculator;
    // while (true) {
    //     calculator.processInstruction(getUserInputString());
    // }

    for (const auto& input : {"a=2+3",
                              "b=e-2",
                              "c=1+2",
                              "d=e/3",
                              "e=a+c",
                              "f=3+4",
                              "undo 2",
                              "e=2+2",
                              "f=g*7",
                              "result",
                              "g=3*2"}) {
        calculator.processInstruction(input);
    }

    return 0;
}