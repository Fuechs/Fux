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
    err.file = &fileName;
    err.source = &error->sources.at("main.fux");
    err.title = "Some test error";
    err.info = "Some direct info";
    Token mTok = Token(IDENTIFIER, "main");
    mTok.end = 4;
    err.subject = &mTok;
    err.report();

    return 0;
}