#pragma once

#include <memory>

#include "AST/Node.h"

class Evaluator
{
public:
    Evaluator(std::shared_ptr<Node> astRootNode);
};
