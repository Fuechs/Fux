#include "../fux.hpp"
#include "../frontend/error/error.hpp"
#include "../frontend/parser/parser.hpp"

class SourceFile {
public:
    SourceFile(const string& filePath, bool mainFile = false) {
        this->filePath = filePath;
        this->fileDir = getDirectory(filePath);
        this->contents = readFile(filePath);
        this->mainFile = mainFile;
    }

    ~SourceFile() {
        filePath.clear();
        fileDir.clear();
        contents.clear();
        error->panic();
        delete parser;
    }

    AST *parse() {
        error = new ErrorManager(filePath, vector<string>());
        parser = new Parser(error, filePath, contents, mainFile);
        AST *root = parser->parse();
        delete this;
        return root;
    }

private:
    ErrorManager *error;
    Parser *parser;

    string filePath;
    string fileDir;
    string contents;

    bool mainFile;
};