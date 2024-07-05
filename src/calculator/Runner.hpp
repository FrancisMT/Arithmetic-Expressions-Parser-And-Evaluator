#pragma once

#include <string>

#include "State.hpp"

namespace Calculator {

class Runner
{
public:
    Runner();
    ~Runner();

    void processInstruction(const std::string& input);

private:
    State mState;
};

} // namespace Calculator