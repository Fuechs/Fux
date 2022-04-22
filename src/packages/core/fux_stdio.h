#ifndef fux_stdio_h
#define fux_stdio_h

#include <stdio.h>

typedef enum { // just for _stdin()
    STRING,
    CHAR,
} Type;

/* 
    * 2022-04-22
    * Fuechs
    Outputs message to stdout
    ? Parameters: message(char*)
    ? Returns: none;
*/
void _stdout(const char* message) {
    puts(message);
}

/*
    * 2022-04-22
    * Fuechs
    Registers input of stdin
    ? Parameters: type(Type)
    ? Returns: input(void*)
*/
void* _stdin(const Type type) {
    switch (type) {
        case STRING: return gets("%s");
        case CHAR: return gets("%c");
    }
    return NULL;
}

#endif