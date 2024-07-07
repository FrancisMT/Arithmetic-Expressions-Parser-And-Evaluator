#pragma once

#include <string>

#include "State.hpp"

namespace Calculator {

class Runner
{
public:
    Runner();
    ~Runner();

    std::vector<std::string> processInstruction(const std::string& input);

private:
    State mState;
};

} // namespace Calculator