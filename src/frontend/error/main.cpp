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

    ParseError err = ParseError();
    err.flags = { ParseError::AGGRESSIVE, ParseError::WARNING, };
    err.type = ParseError::GENERIC;
    err.title = "Some Shitty Error";
    err.subject.info = "Some Test Info";
    err.subject.meta = Metadata(&fileName, &error->sources[fileName], 1, 1, 1, 4);
    err.report();

    return 0;
}