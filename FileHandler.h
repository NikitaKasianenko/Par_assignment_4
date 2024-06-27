#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <cstdlib>
#include "Text.h"

class FileHandler {
public:
	static void write_in_file(const char* path, Text& array, int nrow);
	static char** read_from_file(const char* path, size_t buffersize, int* nrow);
};

#endif