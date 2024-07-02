#include "Text.h"
#include <cstdlib>
#include <cstdio>

Text::Text() {
    buffersize = 256;
    initialrowcount = 10;
    nrow = 0;
    ncol = 0;
    array = nullptr;
    initialize_array();
}

Text::~Text() {
    freeArray();
}

char** Text::getArray() const {
    return array;
}

void Text::setArray(char** newArray) {
    freeArray();
    array = newArray;
    nrow = 0;
    if (newArray != nullptr) {
        while (newArray[nrow] != nullptr) {
            nrow++;
        }
    }
}

int Text::getNrow() const {
    return nrow;
}

void Text::setNrow(int newNrow) {
    nrow = newNrow;
}

int Text::getNCol() const {
    return ncol;
}

void Text::setNCol(int newNcol) {
    ncol = newNcol;
}

void Text::initialize_array() {
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

void Text::freeArray() {
    if (array) {
        for (int i = 0; i < initialrowcount && array[i] != nullptr && array[i] != NULL; i++) {
            free(array[i]);
        }
        array = nullptr;
    }
}

void Text::reallocate_rows() {
    int newInitRows = initialrowcount * 2; 
    char** temp = (char**)realloc(array, newInitRows * sizeof(char*));
    if (temp == nullptr) {
        printf("memory allocation failed.");
        exit(1);
    }
    array = temp;

    for (int i = initialrowcount; i < newInitRows; i++) {
        array[i] = (char*)malloc(buffersize * sizeof(char));
        if (array[i] == nullptr) {
            printf("memory allocation failed.");
            exit(1);
        }
        array[i][0] = '\0'; 
    }

    initialrowcount = newInitRows; 
}


void Text::newbuffer(size_t* buffersize) {
    *buffersize *= 2;
}

int Text::getInitRows() const {
    return initialrowcount;
}
