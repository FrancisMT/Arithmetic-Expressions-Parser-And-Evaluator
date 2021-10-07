# Arithmetic Expressions Parser And Evaluator
A C++ program that can parse and evaluate arithmetic expressions

## Tools
* C++17
* Cmake
* GTest

## Usage
```
❯ ./Arithmetic-Expressions-Parser-Evaluator
Input Arithmetic expression to evaluate: (4 + 5 * (7 - 3)) - 2
Validating input string: (4 + 5 * (7 - 3)) - 2
Validated String: ((4+5*(7-3))-2)
Generating Abstract Syntax Tree
Generated Abstract Syntax Tree
-
    +
        4
        *
            5
            -
                7
                3
    2
Arithmetic Expression Result: 22

```

## Coverage
```
❯ ctest
Test project /home/Projects/Arithmetic-Expressions-Parser-And-Evaluator/cmake-build-debug
      Start  1: EvaluatorUnitTest.evaluatorThrowsIfInputIsNull
 1/12 Test  #1: EvaluatorUnitTest.evaluatorThrowsIfInputIsNull .....................................   Passed    0.00 sec
      Start  2: EvaluatorUnitTest.evaluatorOutputsCorrectResult
 2/12 Test  #2: EvaluatorUnitTest.evaluatorOutputsCorrectResult ....................................   Passed    0.00 sec
      Start  3: ParserUnitTest.parserThrowsWhenInputIsEmpty
 3/12 Test  #3: ParserUnitTest.parserThrowsWhenInputIsEmpty ........................................   Passed    0.00 sec
      Start  4: ParserUnitTest.parserThrowsWhenNegativeNumbersAreProvided
 4/12 Test  #4: ParserUnitTest.parserThrowsWhenNegativeNumbersAreProvided ..........................   Passed    0.00 sec
      Start  5: ParserUnitTest.parserThrowsWhenLiteralsAreTooLarge
 5/12 Test  #5: ParserUnitTest.parserThrowsWhenLiteralsAreTooLarge .................................   Passed    0.00 sec
      Start  6: ParserUnitTest.parserThrowsWhenParenthesesAreNotPaired
 6/12 Test  #6: ParserUnitTest.parserThrowsWhenParenthesesAreNotPaired .............................   Passed    0.00 sec
      Start  7: ParserUnitTest.parserThrowsWhenParenthesesAreIncorrectlyUsed
 7/12 Test  #7: ParserUnitTest.parserThrowsWhenParenthesesAreIncorrectlyUsed .......................   Passed    0.00 sec
      Start  8: ParserUnitTest.parserThrowsWhenOperatorsAreMissing
 8/12 Test  #8: ParserUnitTest.parserThrowsWhenOperatorsAreMissing .................................   Passed    0.00 sec
      Start  9: ParserUnitTest.parserThrowsWhenOperatorsAreIncorrectlyUsed
 9/12 Test  #9: ParserUnitTest.parserThrowsWhenOperatorsAreIncorrectlyUsed .........................   Passed    0.00 sec
      Start 10: ParserUnitTest.parserDoesNotThrowWhenValidInputIsProvided
10/12 Test #10: ParserUnitTest.parserDoesNotThrowWhenValidInputIsProvided ..........................   Passed    0.00 sec
      Start 11: ParserUnitTest.parserGenerasteAST
11/12 Test #11: ParserUnitTest.parserGenerasteAST ..................................................   Passed    0.00 sec
      Start 12: ArithmeticExpressionsHandlingTest.outputsTheCorrectResultOfArithmeticExpressions
12/12 Test #12: ArithmeticExpressionsHandlingTest.outputsTheCorrectResultOfArithmeticExpressions ...   Passed    0.00 sec

100% tests passed, 0 tests failed out of 12

Total Test time (real) =   0.05 sec
```
