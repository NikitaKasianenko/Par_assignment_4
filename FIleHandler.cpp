#define _CRT_SECURE_NO_WARNINGS

#include "FileHandler.h"
#include <stdio.h>
#include <fstream>
#include "CaesarCipher.h"

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

void FileHandler::read_from_file(Text& array, const char* path, size_t buffersize, int* nrow, int parametr, int key) {
    std::ifstream file(path, std::ios::binary);

    if (!file) {
        printf("can't open file\n");
        return;
    }

    CaesarCipher cipher;
    file.seekg(0, std::ios::end);
    std::streampos fsize = file.tellg();
    file.seekg(0, std::ios::beg);

    int loops = fsize / buffersize;
    int lastChunk = fsize % buffersize;

    int totalRowsNeeded = loops + (lastChunk > 0 ? 1 : 0);
    *nrow = totalRowsNeeded;

    array.setNrow(totalRowsNeeded);

    while (array.getNrow() > array.getInitRows()) {
        array.reallocate_rows();
    }

    for (int i = 0; i < loops; i++) {
        file.read(array.getArray()[i], buffersize);
        array.getArray()[i][buffersize] = '\0';

        if (parametr == 1) {
            array.getArray()[i] = cipher.encrypt_text(array.getArray()[i], key);
        }
        else if (parametr == 2) {
            array.getArray()[i] = cipher.decrypt_text(array.getArray()[i], key);
        }
    }

    if (lastChunk > 0) {
        file.read(array.getArray()[loops], lastChunk);
        array.getArray()[loops][lastChunk] = '\0';

        if (parametr == 1) {
            array.getArray()[loops] = cipher.encrypt_text(array.getArray()[loops], key);
        }
        else if (parametr == 2) {
            array.getArray()[loops] = cipher.decrypt_text(array.getArray()[loops], key);
        }
    }

    file.close();
}
