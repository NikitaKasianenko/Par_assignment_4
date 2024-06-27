#include "CaesarCipher.h"
#include <iostream>
using namespace std;

CaesarCipher::CaesarCipher() {
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

CaesarCipher::~CaesarCipher() {
    FreeLibrary(handle);
}

char* CaesarCipher::encrypt_text(char* text, int key) {
    char* result = encrypt(text, key);
    result[strlen(result)] = '\0';
    return result;
}

char* CaesarCipher::decrypt_text(char* text, int key) {
    char* result = decrypt(text, key);
    result[strlen(result)] = '\0';
    return result;
}