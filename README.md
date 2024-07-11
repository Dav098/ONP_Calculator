# ONP_Calculator
This C++ program provides functionality to evaluate mathematical expressions in both Reverse Polish Notation (RPN) and infix notation. The user can input expressions in either format, and the program will parse and calculate the result.

# Features
Evaluate RPN expressions: The program can take expressions in Reverse Polish Notation and evaluate them.
Convert and evaluate infix expressions: It can also handle standard infix expressions by converting them to RPN and then evaluating the result.
Error handling: The program includes mechanisms to catch and report various errors such as division by zero, invalid characters, and mismatched parentheses.
#Compilation
To compile the program, use a C++ compiler such as g++:
g++ -o calculator main.cpp -lm
# Usage
Run the compiled executable:
./calculator

Upon running, the program will display a menu with the following options:

ONP mode: Allows the user to enter and evaluate expressions in Reverse Polish Notation.
Standard expression mode: Allows the user to enter and evaluate standard infix expressions.
Exit: Exits the program.
# Code Overview
Libraries and Preprocessor Directives

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <cstring>
#include <cmath>
#include <vector>

The program includes standard C++ libraries for input/output operations, string manipulation, and mathematical functions.

# Platform-Specific Macros

#ifdef _WIN32
#define clear() system("cls")
#define pause() system("pause")
#else
#define clear() system("clear")
#define pause() while ('\n' != getchar()); printf ("Press enter to continue..."); getchar()
#endif

These macros handle screen clearing and pausing functionality, providing compatibility across Windows and Unix-like systems.

# Helper Functions
float pop(std::vector<float> *vect)
This function pops the last element from a vector-based stack and returns it.

long double parse_and_calculate_rpn(char *wyr)
This function parses and evaluates an RPN expression. It uses a vector to simulate a stack, performing operations as it encounters operators in the expression.

std::string print_exception_description(int e)
This function returns a human-readable string describing an error based on its code.

# Main Functions
long double calculate(char wyr[])
This function converts an infix expression to RPN and evaluates it. It uses stacks to manage operators and parentheses, ensuring proper precedence and associativity.

void calculate_rpn()
Handles user input for RPN expressions, invoking parse_and_calculate_rpn to compute the result and displaying any errors encountered.

void calculate_norm()
Handles user input for infix expressions, invoking calculate to convert and compute the result, and displaying any errors encountered.

void menu()
Displays the main menu, allowing the user to choose between RPN mode, standard expression mode, or exit.

int main()
The entry point of the program, calling the menu function to start the user interaction loop.

# Example

RPN Expression

Enter the expression in ONP: 3 4 + 2 * 7 /

Result: 2

Infix Expression

Enter the expression: (3 + 4) * 2 / 7

Result: 2

# Error Handling
The program includes error handling for:

Division by zero
Extra commas in numbers
Missing numbers
Incorrect bracket usage
Letters in expressions
These errors are reported with specific messages to help the user correct their input.

Enjoy using the RPN and Infix Expression Calculator!
