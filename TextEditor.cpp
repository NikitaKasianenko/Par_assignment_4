#include "TextEditor.h"
#include "Text.h"
#include <stdio.h>

void TextEditor::print(Text& redactor){
    for (int i = 0; i < redactor.getNrow(); i++) {
        printf("%s\n", redactor.getArray()[i]);
    }
}