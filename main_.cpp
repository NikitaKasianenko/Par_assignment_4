#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <stack>
#include <fstream>
#include <windows.h>

using namespace std;

class CLI {
public:
    static char* user_input(size_t* buffersize) {
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

    static char* user_file(size_t* buffersize) {
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
};

class Text {
public:
    Text() {
        buffersize = 256;
        initialrowcount = 10;
        nrow = 0;
        ncol = 0;
        array = nullptr;
        initialize_array();
    }

    ~Text() {
        freeArray();
    }

    char** getArray() const {
        return array;
    }

    void setArray(char** newArray) {
        freeArray();
        array = newArray;
        nrow = 0;
        if (newArray != nullptr) {
            while (newArray[nrow] != nullptr) {
                nrow++;
            }
        }
    }

    int getNrow() const {
        return nrow;
    }

    void setNrow(int newNrow) {
        nrow = newNrow;
    }

    int getNCol() const {
        return ncol;
    }

    void setNCol(int newNcol) {
        ncol = newNcol;
    }

    void initialize_array() {
        array = (char**)malloc(initialrowcount * sizeof(char*));
        if (array == nullptr) {
            printf("memory allocation failed.");
            exit(1);
        }
        for (int i = 0; i < initialrowcount; i++) {
            array[i] = (char*)malloc(buffersize * sizeof(char));
            if (array[i] == nullptr) {
                printf("memory allocation failed.");
                exit(1);
            }
            array[i][0] = '\0';
        }
    }

    void freeArray() {
        if (array) {
            for (int i = 0; i < initialrowcount && array[i] != nullptr; i++) {
                free(array[i]);
            }
            free(array);
            array = nullptr;
        }
    }

    void reallocate_rows() {
        initialrowcount *= 2;
        char** temp = (char**)realloc(array, initialrowcount * sizeof(char*));
        if (temp == nullptr) {
            printf("memory allocation failed.");
            exit(1);
        }
        array = temp;
        for (int i = nrow; i < initialrowcount; i++) {
            array[i] = (char*)malloc(buffersize * sizeof(char));
            if (array[i] == nullptr) {
                printf("memory allocation failed.");
                exit(1);
            }
            array[i][0] = '\0';
        }
    }

    void newbuffer(size_t* buffersize) {
        *buffersize *= 2;
    }

protected:
    size_t buffersize;
    int initialrowcount;
    int nrow;
    int ncol;
    char** array;
};

