#define _CRT_SECURE_NO_WARNINGS

#include "CLI.h"
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <string>
#include "Text.h"

char* CLI::user_input(size_t* buffersize) {
    char* input = (char*)malloc(*buffersize * sizeof(char));
    if (input == nullptr) {
        printf("memory allocation failed.");
        exit(1);
    }

    int length = 0;
    int symbol;

    while ((symbol = getchar()) != '\n' && symbol != EOF) {
        if (length >= *buffersize - 1) {
            *buffersize *= 2;
            input = (char*)realloc(input, *buffersize * sizeof(char));
            if (input == nullptr) {
                printf("memory allocation failed.");
                exit(1);
            }
        }
        input[length++] = symbol;
    }
    input[length] = '\0';

    return input;

}


char* CLI::user_file(size_t* buffersize) {
    char* input = user_input(buffersize);
    char* path = (char*)malloc(256 * sizeof(char));
    if (path == nullptr) {
        printf("memory allocation failed.");
        exit(1);
    }
    sprintf(path, "c:\\windows\\temp\\%s.txt", input);
    free(input);
    return path;

}

void CLI::sscan_user_input(Text array, int* row, int* col, size_t* bufferSize) {
    char* input = nullptr;
    int currow, curcol;

    while (true) {
        printf("Choose line and index: ");
        input = user_input(bufferSize);

        if (strlen(input) > 0 && sscanf(input, "%d %d", &currow, &curcol) == 2) {
            if (currow >= 0 && currow <= array.getNrow() && curcol >= 0 && curcol <= (int)strlen(array.getArray()[currow])) {
                *row = currow;
                *col = curcol;
                free(input);
                return; 
            }
        }

        free(input);
        printf("Choose correct index separated by space in format 'x y'\n");
    }
}

