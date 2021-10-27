#include "gtest/gtest.h"

#include "Evaluator/Evaluator.h"
#include "Parser/Parser.h"

/**
 * Tests that the program is able to correctly parse and evaluate arithmetic expressions
 */
TEST(ArithmeticExpressionsHandlingTest, outputsTheCorrectResultOfArithmeticExpressions)
{

    const auto expressionsAndResultsList = {std::pair{"(4 + 5 * (7 - 3)) - 2", 22},
                                            std::pair{"4+5+7/2", 12},
                                            std::pair{"2+3*(1-2)", -1},
                                            std::pair{"5+(1*2)", 7},
                                            std::pair{"2+3* 1 - 2", 3},
                                            std::pair{"7+3*(1/(2/(3+1)-1))", 1},
                                            std::pair{"(2*(3+6/2)/4)", 3}};

    for (const auto& [arithmeticExpressions, expectedResult] : expressionsAndResultsList) {
        Parser parser(arithmeticExpressions);
        ASSERT_NO_THROW(parser.execute());

        Evaluator evaluator(parser.getAST());
        ASSERT_EQ(evaluator.execute(), expectedResult);
    }
}