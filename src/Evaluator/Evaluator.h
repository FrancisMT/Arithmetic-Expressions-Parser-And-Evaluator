#pragma once

#include <memory>

#include "AST/Node.h"

class Evaluator
{
public:
    explicit Evaluator(std::shared_ptr<AST::Node> astRootNode);

    int32_t execute();

private:
    int32_t traverseAST(const std::shared_ptr<AST::Node>& node);

private:
    std::shared_ptr<AST::Node> mAstRootNode;
};
