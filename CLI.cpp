#define _CRT_SECURE_NO_WARNINGS

#include "CLI.h"
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <string>
#include "Text.h"
#include "FileHandler.h"
#include "CaesarCipher.h"
#include "TextEditor.h"


void CLI::run() {
    size_t bufferSize = 256;
    Text array;
    TextEditor editor;
    CaesarCipher* cipher = nullptr;

    try {
        cipher = new CaesarCipher();
    }catch (const std::exception& e) {
        printf("error: %s\n", e.what());
    }

    char* input = nullptr;
    char* command = nullptr;
    char* path_in = nullptr;
    char* path_out = nullptr;
    int nrow = 0;
    char** buffer = nullptr;
    int row, col, amount, key_chip;

    editor.help();
    while (true) {
        
        int cmd = user_command(&bufferSize);
        input = nullptr;

        if (cmd == 1) {
            printf("Enter text to append: ");
            input = user_input(&bufferSize);
            editor.append_text(array, input, &bufferSize);
        }
        else if (cmd == 2) {
            editor.new_line(array);
        }
        else if (cmd == 3) {
            printf("Enter file name to save: \n");
            path_out = user_file(&bufferSize);
            FileHandler::write_in_file(path_out, array, array.getNrow());
            free(path_out);
        }
        else if (cmd == 4) {
            printf("Enter file name to open: \n");
            path_in = user_file(&bufferSize);
            FileHandler::read_from_file(array, path_in, bufferSize, &nrow, 0, 0);
            free(path_in);
        }
        else if (cmd == 5) {
            editor.print(array);
        }
        else if (cmd == 6) {
            sscan_user_input(array,&row,&col,&bufferSize);
            editor.insert_text(array, &bufferSize, row, col);
        }
        else if (cmd == 7) {
            editor.search(array, &bufferSize);
        }
        else if (cmd == 8) {
            sscan_user_input_amount(array, &row, &col, &amount, &bufferSize);
            editor.delete_text(array, row, col, amount);
        }
        else if (cmd == 9) {
            sscan_user_input_amount(array, &row, &col, &amount, &bufferSize);
            editor.cut(array, row, col, amount);
        }
        else if (cmd == 10) {
            free(command);
            return;
        }
        else if (cmd == 11) {
            system("cls");
            editor.help();
        }
        else if (cmd == 12) {
            sscan_user_input(array, &row, &col, &bufferSize);
            editor.paste(array, row, col, &bufferSize);
        }
        else if (cmd == 13) {
            sscan_user_input_amount(array, &row, &col, &amount, &bufferSize);
            editor.copy(array, row, col, amount);
        }
        else if (cmd == 14) {
            editor.undo(array, bufferSize);
        }
        else if (cmd == 15) {
            editor.redo(array, bufferSize);
        }
        else if (cmd == 16) {
            sscan_user_input(array,&row,&col,&bufferSize);
            editor.insert_rp(array, &bufferSize, row, col);
        }

        else if (cmd == 17) {

            if (cipher) {
                printf("Enter key to encrypt Text: ");
                scanf("%d", &key_chip);
                getchar();
                editor.encrypt_array(array, key_chip, 0);
            }

            else {
                printf("Encryption functionality is not available.\n");
            }

        }

        else if (cmd == 18) {

            if (cipher) {
                printf("Enter key to decrypt Text: ");
                scanf("%d", &key_chip);
                getchar();
                editor.encrypt_array(array, key_chip, 1);
            }

            else {
                printf("Decryption functionality is not available.\n");
            }

        }

        else if (cmd == 19) {

            if (cipher) {
                printf("Enter file name to open: \n");
                path_in = user_file(&bufferSize);
                printf("Enter key to encrypt Text: ");
                scanf("%d", &key_chip);
                getchar();
                FileHandler::read_from_file(array, path_in, bufferSize, &nrow, 1, key_chip);
                free(path_in);
            }

            else {
                printf("Encryption functionality is not available.\n");
            }
        }

        else if (cmd == 20) {

            if (cipher) {
                printf("Enter file name to open: \n");
                path_in = user_file(&bufferSize);
                printf("Enter key to encrypt Text: ");
                scanf("%d", &key_chip);
                getchar();
                FileHandler::read_from_file(array, path_in, bufferSize, &nrow, 2, key_chip);
                free(path_in);
            }

            else {
                printf("Encryption functionality is not available.\n");
            }
        }

        else {
            printf("Invalid command.\n");
        }

        free(command);
        command = nullptr;
    }
}

int CLI::user_command(size_t* buffersize) {
    printf("Choose the command: ");
    char* command = user_input(buffersize);
    int cmd = atoi(command);
    return cmd;
}


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

void CLI::sscan_user_input(Text& array, int* row, int* col, size_t* bufferSize) {
    int currow, curcol;
    char* input = nullptr;


    while (true) {

        printf("Choose line and index: ");
        input = user_input(bufferSize);
        if (sscanf(input, "%d %d", &currow, &curcol) == 2) {
            if (currow >= 0 && currow <= array.getNrow() && curcol >= 0 && curcol <= (int)strlen(array.getArray()[currow])) {
                *row = currow;
                *col = curcol;
                break;
            }
        }

        free(input);
        input = nullptr;
        printf("Choose correct index separated by space in format 'x y'\n");
    }
}

void CLI::sscan_user_input_amount(Text& array, int* row, int* col, int* amount, size_t* bufferSize) {
    int currow, curcol,amount_;
    char* input = nullptr;

    while (true) {
        printf("Choose line, index and number of symbols: ");
        input = user_input(bufferSize);
        if (sscanf(input, "%d %d %d", &currow, &curcol, &amount_) == 3) {
            if (currow >= 0 && currow <= array.getNrow() && curcol >= 0 && curcol < (int)strlen(array.getArray()[currow]) &&
                amount_ >= 0 && amount_ + curcol <= (int)strlen(array.getArray()[currow])) {
                *row = currow;
                *col = curcol;
                *amount = amount_;
                break;
            }
        }

        free(input);
        input = nullptr;
        printf("Choose correct index and amount of symbols separated by space in format 'x y z'\n");
    }

}

