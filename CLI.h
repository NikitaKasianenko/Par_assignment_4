#ifndef CLI_H
#define CLI_H

#include <cstdlib>
#include "Text.h"

class CLI {
public:
    static char* user_input(size_t* buffersize);
    static char* user_file(size_t* buffersize);
    static void sscan_user_input(Text array, int* row, int* col, size_t* bufferSize);
    static void sscan_user_input_amount(Text array, int* row, int* col, int* amount, size_t* bufferSize);

};
#endif 
