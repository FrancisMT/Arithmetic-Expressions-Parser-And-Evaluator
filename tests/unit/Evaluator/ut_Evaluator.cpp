#include "gtest/gtest.h"

#include "Evaluator/Evaluator.h"

/**
 * Tests that the Evaluator throws an exception when the provided input is a nullptr
 */
TEST(EvaluatorUnitTest, evaluatorThrowsIfInputIsNull)
{
    Evaluator evaluator(nullptr);
    ASSERT_THROW(evaluator.execute(), std::invalid_argument);
}

/**
 * Tests that the Evaluator is able to calculate the correct integer result from ASTs with data from
 * arithmetic expressions
 */
TEST(EvaluatorUnitTest, evaluatorOutputsCorrectResult)
{
    // Generating valid AST for the arithmetic expression: "4+5+7/2"
    using namespace AST;
    const auto rootNode = std::make_unique<Node>(
          // First Level
          '+',
          // Second Level
          std::make_unique<Node>('+',
                                 // Third Level
                                 std::make_unique<Node>('4'),
                                 std::make_unique<Node>('5')),
          // Second Level
          std::make_unique<Node>('/',
                                 // Third Level
                                 std::make_unique<Node>('7'),
                                 std::make_unique<Node>('2')));

    Evaluator evaluator(rootNode);
    ASSERT_NO_THROW(evaluator.execute());
    ASSERT_EQ(evaluator.execute(), 12);
}
