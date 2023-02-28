#include "error.hpp"

int main(void) {
    ErrorManager *error = new ErrorManager();
    string fileName = "main.fux";
    error->addSourceFile(fileName, {
        "(foo + bar) | (1.5 / 3)"
    });
    error->simpleError(ParseError::ILLEGAL_OPERANDS, 
        "Invalid operand type for 'bitwise or' operation.", 
        fileName, 1, 1, 15, 23,
        "RHS with invalid type 'f64'", 13, "Binary Operator");
    return 0;
}