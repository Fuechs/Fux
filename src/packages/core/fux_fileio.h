#ifndef fux_fileio_h
#define fux_fileio_h

#include <stdio.h>
#include <string.h>

typedef enum {
    READ,
    WRITE,
    APPEND,
} fileop;

FILE* _file_open(const char* filepath, fileop mode) {
    switch (mode) {
        case READ: return fopen(filepath, "r");
        case WRITE: return fopen(filepath, "w");
        case APPEND: return fopen(filepath, "a");
    }
}

#endif