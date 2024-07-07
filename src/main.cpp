
#include <iostream>
#include <string>

#include "calculator/Runner.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Calculator::Runner calculator;

    // const auto getUserInputString = [] {
    //     std::cout << "\nInput Arithmetic expression to evaluate: ";
    //     std::string input;
    //     std::getline(std::cin, input);

    //     return input;
    // };

    // while (true) {
    //     const auto operationResults = calculator.processInstruction(getUserInputString());

    //     for (auto itr = operationResults.cbegin(); itr != operationResults.cend(); ++itr) {
    //         std::cout << *itr << (std::next(itr) != operationResults.cend() ? ", " : "\n");
    //     }
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

        const auto operationResults = calculator.processInstruction(input);

        for (auto itr = operationResults.cbegin(); itr != operationResults.cend(); ++itr) {
            std::cout << *itr << (std::next(itr) != operationResults.cend() ? ", " : "\n");
        }
    }

    return 0;
}