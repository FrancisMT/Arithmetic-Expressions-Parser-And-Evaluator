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

    std::shared_ptr<AST::Node> getAST();

private:
    void validateInput();
    void createAST();

private:
    std::string mInputString;

    std::stack<char> operatorStack;
    std::stack<std::shared_ptr<AST::Node>> valueStack;
};
