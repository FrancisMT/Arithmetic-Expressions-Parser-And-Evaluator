#include "gtest/gtest.h"

#include "Parser/Parser.h"

using namespace ::testing;

/**
 * @brief Test fixture for the Parser
 */
class ParserUnitTest : public Test
{
protected:
    /**
     * @brief Method used to validate whether parser inputs are valid or not
     *
     * @param isSuccessScenario True if a success scenario is to be tested, false otherwise
     */
    void testInputs(bool isSuccessScenario = true)
    {
        for (const auto& inputString : mTestInputs) {
            Parser parser(inputString);

            if (isSuccessScenario) {
                ASSERT_NO_THROW(parser.execute());
            } else {
                ASSERT_THROW(parser.execute(), std::invalid_argument);
            }
        }
    }

protected:
    /// List of inputs to be tested
    std::vector<std::string> mTestInputs;
};

/**
 * Tests that the Parser throws an exception when the input string is empty
 */
TEST_F(ParserUnitTest, parserThrowsWhenInputIsEmpty)
{
    mTestInputs = {""};
    testInputs(false);
}

/**
 * Tests that the Parser throws an exception when the input has negative numbers
 */
TEST_F(ParserUnitTest, parserThrowsWhenNegativeNumbersAreProvided)
{
    mTestInputs = {"-1", "-22", "2+(-3*2)", "(-5)", "-(2*3)"};
    testInputs(false);
}

/**
 * Tests that the Parser throws an exception when the input has literals that are too large
 */
TEST_F(ParserUnitTest, parserThrowsWhenLiteralsAreTooLarge)
{
    mTestInputs = {"42", "1337", "11*11+3-(20)", "10  +  1"};
    testInputs(false);
}

/**
 * Tests that the Parser throws an exception when the input has parentheses that do not come in
 * pairs
 */
TEST_F(ParserUnitTest, parserThrowsWhenParenthesesAreNotPaired)
{
    mTestInputs = {"(1+2))", "(3*  3", "4+2*(5/7))"};
    testInputs(false);
}

/**
 * Tests that the Parser throws an exception when the input has parentheses that are incorrectly
 * used
 */
TEST_F(ParserUnitTest, parserThrowsWhenParenthesesAreIncorrectlyUsed)
{
    mTestInputs = {")1+2(", "(3*()3)", "4+2)(*(5/7)"};
    testInputs(false);
}

/**
 * Tests that the Parser throws an exception when the input has missing operators
 */
TEST_F(ParserUnitTest, parserThrowsWhenOperatorsAreMissing)
{
    mTestInputs = {"(4 + 5 (7 - 3)) - 2", "2+(5*3)7"};
    testInputs(false);
}

/**
 * Tests that the Parser throws an exception when the input has operators used incorrectly
 */
TEST_F(ParserUnitTest, parserThrowsWhenOperatorsAreIncorrectlyUsed)
{
    mTestInputs = {"(4 +* 5 - (7 - 3)) - 2", "2+(/5*3)7", "*(4+5)-3", "+-42", "1-3+3/7+"};
    testInputs(false);
}

/**
 * Tests that the Parser does not throw an exception when the input has the correct syntax
 */
TEST_F(ParserUnitTest, parserDoesNotThrowWhenValidInputIsProvided)
{
    mTestInputs = {"(4 + 5 * (7 - 3)) - 2",
                   "4+5+7/2",
                   "2+3*(1-2)",
                   "5+(1*2)",
                   "2+3* 1 - 2",
                   "7+3*(1/(2/(3+1)-1))",
                   "(2*(3+6/2)/4)"};
    testInputs(true);
}

/**
 * Tests that the Parser is able to generate an AST with the correct number of nodes
 */
TEST_F(ParserUnitTest, parserGenerasteAST)
{
    Parser parser("5+(1*2)");
    ASSERT_NO_THROW(parser.execute());
    ASSERT_NE(parser.getAST(), nullptr);
    ASSERT_EQ(AST::getNumberOfNodes(parser.getAST().get()), 5);

    using namespace AST;
    const auto rootNode
          = std::make_unique<Node>('+', // First Level
                                   std::make_unique<Node>('5'),
                                   std::make_unique<Node>('*', // Second Level
                                                          std::make_unique<Node>('1'),
                                                          std::make_unique<Node>('2')));

    ASSERT_TRUE(areASTsIdentical(rootNode.get(), parser.getAST().get()));
}
