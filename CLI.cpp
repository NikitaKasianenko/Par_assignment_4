#define _CRT_SECURE_NO_WARNINGS

#include "CLI.h"
#include <cstdlib>
#include <cstdio>

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
