# Arithmetic Expressions Parser And Evaluator
A C++ program that can parse and evaluate arithmetic expressions


## Challenge description

>Create a C++ program that can parse and evaluate arithmetic expressions.
>Parsing and evaluating should be two independent steps that are separated
>by a data structure, the abstract syntax tree (AST).
>
>The core program should not use anything apart from the C++17 STL.
>Please also refrain from using regular expressions, they usually
>complicate this specific task.
>
>Frameworks for building and testing can be used, though.
>
>Arithmetic expressions consist of decimal integers (to simplify,
>only single-digit numbers from 0 to 9 are allowed as literals, i.e. neither
>multi-digit nor fractional numbers are allowed as literals).
>combined using the binary operators +, *, /, - and parentheses
>and should respect the usual precedence relations.
>
>Since all tokens consist of single characters only, the parser
>can be implemented without a tokenizer. Furthermore, a simple
>top-down parser should be sufficient, implementing the shunting-yard
>algorithm is not required (we assume that the machine stack is
>large enough).
>
>The program should detect invalid input and report an error
>(a case-specific error message or source location is not necessary).
>
>Evaluation should be performed on integers.
>
>Unit tests are appreciated. You can use a framework of you choice for that,
>or no framework at all.
>
>Examples:
>
>On input `(4 + 5 * (7 - 3)) - 2`, the program should output `22`
>
>On input `4+5+7/2` it should output `12`.
>
>The input `10 + 1` is invalid (literal is too large)
>
>The input `-10` is invalid (either negative literal or unary minus)


## Tools
* C++17
* Cmake
* GTest

## Build
```
❯ mkdir build && cd build
❯ cmake ..
❯ cmake --build . --config debug
```

## Usage
```
❯ ./Calculator-Challenge
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

### Install GTest
```shell
sudo apt install libgtest-dev
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
cd lib
sudo cp *.a /usr/lib
```

### Run tests
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
