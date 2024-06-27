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

class TextEditor {
public:
    void print(Text& redactor) {
        for (int i = 0; i < redactor.getNrow(); i++) {
            printf("%s\n", redactor.getArray()[i]);
        }
    }
};

class CaesarCipher {
public:
    CaesarCipher() {
        handle = LoadLibrary(TEXT("c:\\users\\nekit\\source\\repos\\assignment_3\\assignment_3\\caesar.dll"));
        if (handle == nullptr) {
            cout << "library not found or failed to load" << endl;
            exit(1);
        }

        encrypt = (encrypt_ptr)GetProcAddress(handle, "encrypt");
        decrypt = (decrypt_ptr)GetProcAddress(handle, "decrypt");

        if (encrypt == nullptr || decrypt == nullptr) {
            cout << "function 'encrypt' or 'decrypt' not found in the library" << endl;
            FreeLibrary(handle);
            exit(1);
        }
    }

    ~CaesarCipher() {
        FreeLibrary(handle);
    }

    char* encrypt_text(char* text, int key) {
        return encrypt(text, key);
    }

    char* decrypt_text(char* text, int key) {
        return decrypt(text, key);
    }

private:
    HINSTANCE handle;
    typedef char* (*encrypt_ptr)(char*, int);
    typedef char* (*decrypt_ptr)(char*, int);
    encrypt_ptr encrypt;
    decrypt_ptr decrypt;
};

class FileHandler {
public:
    static void write_in_file(const char* path, Text& array, int nrow) {
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

    static char** read_from_file(const char* path, size_t buffersize, int* nrow) {
        std::ifstream file(path);
        if (!file) {
            printf("can't open file\n");
            return nullptr;
        }

        char** buffer = (char**)malloc(buffersize * sizeof(char*));
        if (buffer == nullptr) {
            printf("memory allocation failed.");
            return nullptr;
        }

        std::string line;
        *nrow = 0;
        while (getline(file, line)) {
            buffer[*nrow] = (char*)malloc((line.size() + 1) * sizeof(char));
            if (buffer[*nrow] == nullptr) {
                printf("memory allocation failed.");
                return nullptr;
            }
            strcpy(buffer[*nrow], line.c_str());
            (*nrow)++;
            if (*nrow >= buffersize) {
                buffersize *= 2;
                buffer = (char**)realloc(buffer, buffersize * sizeof(char*));
                if (buffer == nullptr) {
                    printf("memory allocation failed.");
                    return nullptr;
                }
            }
        }

        buffer[*nrow] = nullptr;

        return buffer;
    }
};

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
        case 4: // Exit
            free(command);
            return 0;

        case 3: // Print state
            editor.print(array);
            free(command);
            continue;

        case 1: // Encrypt
        case 2: // Decrypt
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
            break;
        }

        free(command);
        free(path_in);
    }

    return 0;
}
