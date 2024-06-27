#ifndef CLI_H
#define CLI_H

#include <cstdlib>

class CLI {
public:
    static char* user_input(size_t* buffersize);
    static char* user_file(size_t* buffersize);
};

#endif 
