#define _CRT_SECURE_NO_WARNINGS

#include "FileHandler.h"
#include <stdio.h>
#include <fstream>

void FileHandler::write_in_file(const char* path, Text& array, int nrow) {
    FILE* file = fopen(path, "w");

    if (file == nullptr) {
        printf("can't open file\n");
        return;
    }

    for (int i = 0; i < nrow; i++) {
        fprintf(file, "%s\n", array.getArray()[i]);
    }
    printf("successful\n");
    fclose(file);
}

char** FileHandler::read_from_file(const char* path, size_t buffersize, int* nrow) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        printf("can't open file\n");
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    std::streampos fsize = file.tellg();
    file.seekg(0, std::ios::beg);

    int loops = fsize / buffersize;
    int lastChunk = fsize % buffersize;

    char** buffer = (char**)malloc((loops + 1) * sizeof(char*));
    if (buffer == nullptr) {
        printf("memory allocation failed.");
        return nullptr;
    }

    for (int i = 0; i < loops; i++) {
        buffer[i] = (char*)malloc(buffersize * sizeof(char));
        if (buffer[i] == nullptr) {
            printf("memory allocation failed.");
            return nullptr;
        }
        file.read(buffer[i], buffersize);
        buffer[i][buffersize - 1] = '\0';
    }

    if (lastChunk > 0) {
        buffer[loops] = (char*)malloc((lastChunk * sizeof(char)) + 1);
        if (buffer[loops] == nullptr) {
            printf("memory allocation failed.");
            return nullptr;
        }
        file.read(buffer[loops], lastChunk);
        buffer[loops][lastChunk] = '\0';

    }

    *nrow = loops + (lastChunk > 0 ? 1 : 0);
    buffer[*nrow] = nullptr;

    return buffer;

}