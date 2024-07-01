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
    char* input = nullptr;
    char* command = nullptr;
    char* path_in = nullptr;
    char* path_out = nullptr;
    int key = 0;
    int nrow = 0;
    char** buffer = nullptr;

    editor.help();
    while (true) {
        printf("Choose the command: ");
        command = CLI::user_input(&bufferSize);
        int cmd = atoi(command);

        if (cmd == 1) {

            printf("Enter text to append: ");
            input = CLI::user_input(&bufferSize);
            editor.append_text(array, input, &bufferSize);
            input = nullptr;
        }
        else if (cmd == 2) {
            editor.new_line(array);
        }
        else if (cmd == 3) {

            printf("Enter file name to save: \n");
            path_out = CLI::user_file(&bufferSize);
            FileHandler::write_in_file(path_out, array, array.getNrow());
            free(path_out);
        }
        else if (cmd == 4) {

            printf("Enter file name to open: \n");
            path_in = CLI::user_file(&bufferSize);
            FileHandler::read_from_file(array,path_in, bufferSize, &nrow,0,0);
            free(path_in);
        }
        else if (cmd == 5) {
            editor.print(array);
        }
        else if (cmd == 6) {

            int row, col;
            printf("Enter row and column to insert text: ");
            scanf("%d %d", &row, &col);
            getchar(); 
            editor.insert_text(array, &bufferSize, row, col);
        }
        else if (cmd == 7) {
            editor.search(array, &bufferSize);
        }
        else if (cmd == 8) {

            int row, col, amount;
            printf("Enter row, column, and amount of text to delete: ");
            scanf("%d %d %d", &row, &col, &amount);
            getchar(); 
            editor.delete_text(array, row, col, amount);
        }
        else if (cmd == 9) {

            int row, col, amount;
            printf("Enter row, column, and amount of text to cut: ");
            scanf("%d %d %d", &row, &col, &amount);
            getchar(); 
            editor.cut(array, row, col, amount);
        }
        else if (cmd == 10) {
            free(command);
            return 0;
        }
        else if (cmd == 11) {
            system("cls"); 
        }
        else if (cmd == 12) {

            int row, col;
            printf("Enter row and column to paste text: ");
            scanf("%d %d", &row, &col);
            getchar();
            editor.paste(array, row, col, &bufferSize);
        }
        else if (cmd == 13) {

            int row, col, amount;
            printf("Enter row, column, and amount of text to copy: ");
            scanf("%d %d %d", &row, &col, &amount);
            getchar(); 
            editor.copy(array, row, col, amount);
        }
        else if (cmd == 14) {
            editor.undo(array, bufferSize);
        }
        else if (cmd == 15) {
            editor.redo(array, bufferSize);
        }
        else if (cmd == 16) {

            int row, col;
            printf("Enter row and column to insert with replacement: ");
            scanf("%d %d", &row, &col);
            getchar(); 
            editor.insert_rp(array, &bufferSize, row, col);
        }
        else if (cmd == 17) {

            int key_chip;
            printf("Enter key to encrypt Text: ");
            scanf("%d", &key_chip);
            getchar();
            editor.encrypt_array(array, key_chip, 0);

        }
        else if (cmd == 18) {

            int key_chip;
            printf("Enter key to decrypt Text: ");
            scanf("%d", &key_chip);
            getchar();
            editor.encrypt_array(array, key_chip, 1);
        }
        else if (cmd == 19) {
            int key_chip;
            printf("Enter file name to open: \n");
            path_in = CLI::user_file(&bufferSize);
            printf("Enter key to encrypt Text: ");
            scanf("%d", &key_chip);
            getchar();
            FileHandler::read_from_file(array, path_in, bufferSize, &nrow, 1, key_chip);
            free(path_in);
        }
        else if (cmd == 20) {

            int key_chip;
            printf("Enter file name to open: \n");
            path_in = CLI::user_file(&bufferSize);
            printf("Enter key to encrypt Text: ");
            scanf("%d", &key_chip);
            getchar();
            FileHandler::read_from_file(array, path_in, bufferSize, &nrow, 2, key_chip);
            free(path_in);

        }


        else {
            printf("Invalid command.\n");
        }

        free(command);
        command = nullptr;
    }

    return 0;
}
