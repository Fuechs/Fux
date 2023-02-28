#include "error.hpp"

int main(void) {
    ErrorManager *error = new ErrorManager();
    string fileName = "main.fux";
    error->addSourceFile(fileName, {
        "main(argc: u64, argv: **c8): i64 {",
        "    puts \"Hello World!\\n\";",
        "    return 0;",
        "",
        "",
        "",
        "",
        "}"
    });
    error->createError(fileName, ParseError::DUPLICATE_SYMBOL, 1, 8, "The symbol 'main' is already taken by another function.", 
        "Try changing the name (e.g. to 'Main', 'mainFunc')");
    error->report();
    return 0;
}