#include "CaesarCipher.h"
#include <iostream>
using namespace std;

CaesarCipher::CaesarCipher() {

    handle = LoadLibrary(TEXT("c:\\users\\nekit\\source\\repos\\assignment_3\\assignment_3\\caesar.dll"));
    if (handle == nullptr) {
        throw exception("run-time Error load library");
    }

    encrypt = (encrypt_ptr)GetProcAddress(handle, "encrypt");
    decrypt = (decrypt_ptr)GetProcAddress(handle, "decrypt");

    if (encrypt == nullptr || decrypt == nullptr) {
        FreeLibrary(handle);
        throw exception("Error load func");
    }
}

CaesarCipher::~CaesarCipher() {
    if (handle) {
        FreeLibrary(handle);
    }
}

char* CaesarCipher::encrypt_text(char* text, int key) {
    if (!encrypt) {
        throw exception("encrypt function not loaded");
    }
    char* result = encrypt(text, key);
    result[strlen(result)] = '\0';
    return result;
}

char* CaesarCipher::decrypt_text(char* text, int key) {
    if (!decrypt) {
        throw exception("decrypt function not loaded");
    }
    char* result = decrypt(text, key);
    result[strlen(result)] = '\0';
    return result;
}