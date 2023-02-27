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
    err.flags = { ParseError::AGGRESSIVE, ParseError::WARNING, ParseError::REFERENCE };
    err.type = ParseError::GENERIC;
    err.title = "You-Are-An-Idiot-Error";
    err.subject.info = "Seriously? How?";
    err.subject.pointer = 10;
    err.subject.pointerText = "I don't like this so the compilation failed. Fuck you.";
    err.subject.meta = Metadata(&fileName, &error->sources[fileName], 1, 1, 5, 27);
    err.reference.meta = Metadata(&fileName, &error->sources[fileName], 1, 8, 1, 1);
    err.reference.info = "Yeah this is kinda the whole shitty function YOU wrote.";
    err.notes = { "Note: You are a fucking dumbass.", "Help: Try contacting a doctor." };
    err.report();

    return 0;
}