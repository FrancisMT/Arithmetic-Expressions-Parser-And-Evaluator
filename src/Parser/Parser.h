#pragma once
#include <memory>
#include <stack>
#include <string>

#include "AST/Node.h"

class Parser
{
public:
    explicit Parser(std::string&& inputToParse);

    void execute();

    //    void getAST();

private:
    void validateInput();
    void createAST();

private:
    std::string mInputString;

    std::stack<char> operatorStack;
    std::stack<Node*> valueStack;
};
