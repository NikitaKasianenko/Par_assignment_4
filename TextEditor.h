#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <cstdlib>
#include "Text.h"
#include "stack"
#include "string"
#include "string.h"


using namespace std;

class TextEditor {
public:
    void print(Text& redactor);
    void append_text(Text& redactor, char* input, size_t* buffersize);
    void new_line(Text& redactor);
    void insert_text(Text& redactor, size_t* buffersize, int currow, int curcol);
    void delete_text(Text& redactor, int currow, int curcol, int amount);
    void search(Text& redactor, size_t* buffersize);
    void cut(Text& redactor, int currow, int curcol, int amount);
    void insert_rp(Text& redactor, size_t* buffersize, int currow, int curcol);
    void copy(Text& redactor, int currow, int curcol, int amount);
    void paste(Text& redactor, int currow, int curcol, size_t* buffersize);
    void undo(Text& redactor, size_t bufferSize);
    void redo(Text& redactor, size_t bufferSize);
    void help();
    void encrypt_array(Text& redactor, int key, int parametr);
private:

    stack<char*> undostack;
    stack<char*> redostack;
    stack<char*> bufferstack;
    stack<char*> bufferundostack;
};

#endif 
