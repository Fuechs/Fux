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

    ParseError err = ParseError({ParseError::REFERENCE}, ParseError::ILLEGAL_TYPE,  
        "Type of returned value does not match declared type of function.", 
        Metadata(&fileName, &error->sources[fileName], 3, 3, 5, 12), "Value of type 'u8' gets returned here.",
        Metadata(&fileName, &error->sources[fileName], 1, 8, 1, 1), "Function got declared with type 'i64' here.", 
        {"Help: Cast the value to the type 'i64'.", "Note: There was no valid type cast from 'u8' to 'i64' found."});
    err.report();

    return 0;
}