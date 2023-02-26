#include "error.hpp"

int main(void) {
    ErrorManager *error = new ErrorManager();
    string fileName = "main.fux";
    error->addSourceFile(fileName, {
        "main(argc: u64, argv: **c8): void {",
        "   puts \"Hello World!\\n\";",
        "   return 0;",
        "}"
    });

    ParseError err = ParseError();
    err.flags = { ParseError::WARNING };
    err.type = ParseError::UNEXPECTED_TOKEN;
    err.title = "Some test error";
    err.info = "Some direct info";
    err.subject = Metadata(&fileName, &error->sources[fileName], 1, 4, 0, 0);
    err.report();

    return 0;
}