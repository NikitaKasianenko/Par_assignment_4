#ifndef CLI_H
#define CLI_H

#include <cstdlib>
#include "Text.h"

class CLI {
public:
     void run();
     char* user_input(size_t* buffersize);
     char* user_file(size_t* buffersize);
     void sscan_user_input(Text& array, int* row, int* col, size_t* bufferSize);
     void sscan_user_input_amount(Text& array, int* row, int* col, int* amount, size_t* bufferSize);
     int user_command(size_t* buffersize);

};
#endif 
