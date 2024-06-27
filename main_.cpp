#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <stack>
#include <fstream>
#include <windows.h>
#include "CLI.h"
#include "Text.h"
#include "TextEditor.h"
#include "CaesarCipher.h"
#include "FileHandler.h"

int main() {
    size_t bufferSize = 256;
    Text array;
    TextEditor editor;
    CaesarCipher cipher;
    char* command = nullptr;
    char* path_in = nullptr;
    char* path_out = nullptr;
    int key = 0;
    int nrow = 0;
    char** buffer = nullptr;

    while (true) {
        printf("Encrypt - 1, Decrypt - 2, Print state - 3, Exit - 4 \nCommand: ");
        command = CLI::user_input(&bufferSize);

        switch (atoi(command)) {
        case 4:
            free(command);
            return 0;

        case 3:
            editor.print(array);
            free(command);
            continue;

        case 1:
        case 2:
            printf("Enter file name to open: \n");
            path_in = CLI::user_file(&bufferSize);

            printf("Enter key: \n");
            key = atoi(CLI::user_input(&bufferSize));
            printf("Enter file name to save: \n");
            path_out = CLI::user_file(&bufferSize);

            buffer = FileHandler::read_from_file(path_in, bufferSize, &nrow);
            if (buffer == nullptr) {
                printf("Error reading from file.\n");
                free(path_in);
                free(path_out);
                free(command);
                continue;
            }
            array.setArray(buffer);
            array.setNrow(nrow);

            for (int i = 0; i < nrow; i++) {
                if (atoi(command) == 1) {
                    array.getArray()[i] = cipher.encrypt_text(array.getArray()[i], key);
                }
                else if (atoi(command) == 2) {
                    array.getArray()[i] = cipher.decrypt_text(array.getArray()[i], key);
                }
            }

            FileHandler::write_in_file(path_out, array, nrow);
            free(path_out);
            break;

        default:
            printf("Invalid command.\n");
        }

        free(command);
        free(path_in);
    }

    return 0;
}
