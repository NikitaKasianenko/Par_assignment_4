#ifndef CAESARCIPHER_H
#define CAESARCIPHER_H

#include <cstdlib>
#include <windows.h>

class CaesarCipher{
public:
    CaesarCipher();
    ~CaesarCipher();

    char* encrypt_text(char* text, int key);
    char* decrypt_text(char* text, int key);

private:
    HINSTANCE handle;
    typedef char* (*encrypt_ptr)(char*, int);
    typedef char* (*decrypt_ptr)(char*, int);
    encrypt_ptr encrypt;
    decrypt_ptr decrypt;
};

#endif
