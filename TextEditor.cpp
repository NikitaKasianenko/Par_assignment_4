#define _CRT_SECURE_NO_WARNINGS

#include "TextEditor.h"
#include "Text.h"
#include <stdio.h>
#include "CLI.h"
#include "CaesarCipher.h"

void TextEditor::print(Text& redactor) {
    for (int i = 0; i <= redactor.getNrow(); i++) {
        printf("%s\n", redactor.getArray()[i]);
    }
}

void TextEditor::encrypt_array(Text& redactor,int key,int parametr) {

    CaesarCipher chipher;
    if (parametr == 0) {
        for (int i = 0; i <= redactor.getNrow(); i++) {
            redactor.getArray()[i] = chipher.encrypt_text(redactor.getArray()[i], key);
        }
    }

    else if (parametr == 1)
    {
        for (int i = 0; i <= redactor.getNrow(); i++) {
            redactor.getArray()[i] = chipher.decrypt_text(redactor.getArray()[i], key);
        }

    }

}

void TextEditor::append_text(Text& redactor, char* input, size_t* buffersize) {
    int ncol = redactor.getNCol();
    int nrow = redactor.getNrow();
    char** array = redactor.getArray();

    if (array == nullptr || array[nrow] == nullptr) {
        printf("Error: array or row is not initialized.\n");
        return;
    }

    if (ncol + strlen(input) >= (int)(*buffersize) - 1) {
        redactor.newbuffer(buffersize);
        array[nrow] = (char*)realloc(array[nrow], *buffersize * sizeof(char));
        if (array[nrow] == nullptr) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
    }

    strcat(array[nrow], input);
    ncol += strlen(input);
    redactor.setNCol(ncol);

    char* undo_info = (char*)malloc((strlen(input) + 3) * sizeof(char));
    sprintf(undo_info, "1\t%s", input);
    undostack.push(undo_info);

    while (!redostack.empty()) {
        free(redostack.top());
        redostack.pop();
    }

    free(input);
}

void TextEditor::new_line(Text& redactor) {
    int nrow = redactor.getNrow();
    redactor.setNrow(nrow + 1);
    redactor.setNCol(0);
    if (redactor.getNrow() >= redactor.getNrow()) {
        redactor.reallocate_rows();
    }

    printf("new line is started\n");

    char* undo_info = (char*)malloc(4 * sizeof(char));
    sprintf(undo_info, "2");
    undostack.push(undo_info);

    while (!redostack.empty()) {
        free(redostack.top());
        redostack.pop();
    }
}

void TextEditor::insert_text(Text& redactor, size_t* buffersize, int currow, int curcol) {
    char* input = nullptr;
    CLI CLI;
    printf("enter text to insert: ");
    input = CLI.user_input(buffersize);

    int text_length = strlen(input);
    if (text_length + strlen(redactor.getArray()[currow]) >= *buffersize) {
        redactor.newbuffer(buffersize);
        redactor.getArray()[currow] = (char*)realloc(redactor.getArray()[currow], *buffersize * sizeof(char));
        if (redactor.getArray()[currow] == nullptr) {
            printf("memory allocation failed");
            free(input);
            exit(1);
        }
    }

    for (int i = strlen(redactor.getArray()[currow]); i >= curcol; i--) {
        redactor.getArray()[currow][i + text_length] = redactor.getArray()[currow][i];
    }

    for (int i = 0; i < text_length; i++) {
        redactor.getArray()[currow][curcol + i] = input[i];
    }

    char* undo_info = (char*)malloc((strlen(input) + *buffersize) * sizeof(char));
    sprintf(undo_info, "6\t%d\t%d\t%s", currow, curcol, input);
    undostack.push(undo_info);

    while (!redostack.empty()) {
        free(redostack.top());
        redostack.pop();
    }
    free(input);
}

void TextEditor::delete_text(Text& redactor, int currow, int curcol, int amount) {
    char* input = nullptr;

    int new_length = (int)strlen(redactor.getArray()[currow]) - amount;
    char* deleted_text = (char*)malloc((amount + 1) * sizeof(char));

    strncpy(deleted_text, &redactor.getArray()[currow][curcol], amount);
    for (int i = curcol; i < new_length; ++i) {
        redactor.getArray()[currow][i] = redactor.getArray()[currow][i + amount];
    }
    redactor.getArray()[currow][new_length] = '\0';
    deleted_text[amount] = '\0';
    redactor.setNCol(redactor.getNCol() - amount);

    char* undo_info = (char*)malloc((amount + redactor.getNCol()) * sizeof(char));
    sprintf(undo_info, "8\t%d\t%d\t%s", currow, curcol, deleted_text);
    undostack.push(undo_info);

    while (!redostack.empty()) {
        free(redostack.top());
        redostack.pop();
    }

    free(deleted_text);
}

void TextEditor::search(Text& redactor, size_t* buffersize) {
    printf("enter text to search: ");
    CLI CLI;
    char* input = CLI.user_input(buffersize);
    char* to_search = input;
    bool found = false;
    char* name = nullptr;

    for (int i = 0; i <= redactor.getNrow(); i++) {
        name = redactor.getArray()[i];
        while ((name = strstr(name, to_search)) != nullptr) {
            printf("substring found at index: %d %d\n", i, (int)(name - redactor.getArray()[i]));
            found = true;
            name++;
        }
    }

    if (!found) {
        printf("substring not found\n");
    }
    free(input);
}

void TextEditor::cut(Text& redactor, int currow, int curcol, int amount) {
    copy(redactor, currow, curcol, amount);
    delete_text(redactor, currow, curcol, amount);

    char* undo_info = (char*)malloc((2) * sizeof(char));
    sprintf(undo_info, "9");
    undostack.push(undo_info);

    while (!redostack.empty()) {
        free(redostack.top());
        redostack.pop();
    }
}

void TextEditor::insert_rp(Text& redactor, size_t* buffersize, int currow, int curcol) {
    char* input = nullptr;
    char* original_text = _strdup(redactor.getArray()[currow]);

    printf("enter text: ");
    CLI CLI;
    input = CLI.user_input(buffersize);

    int text_length = strlen(input);

    if (text_length + strlen(redactor.getArray()[currow]) >= *buffersize) {
        redactor.newbuffer(buffersize);
        redactor.getArray()[currow] = (char*)realloc(redactor.getArray()[currow], *buffersize * sizeof(char));
        if (redactor.getArray()[currow] == nullptr) {
            printf("memory allocation failed");
            free(input);
            exit(1);
        }
    }

    for (int i = 0; i < text_length; i++) {
        redactor.getArray()[currow][curcol + i] = input[i];
    }
    redactor.getArray()[currow][curcol + text_length] = '\0';

    char* undo_info = (char*)malloc((strlen(original_text) + *buffersize) * sizeof(char));
    sprintf(undo_info, "16\t%d\t%d\t%s", currow, curcol, original_text);
    undostack.push(undo_info);

    free(original_text);
    free(input);

    while (!redostack.empty()) {
        free(redostack.top());
        redostack.pop();
    }
}

void TextEditor::copy(Text& redactor, int currow, int curcol, int amount) {
    char* input = nullptr;

    char* text = (char*)malloc((amount + 1) * sizeof(char));
    strncpy(text, &redactor.getArray()[currow][curcol], amount);
    text[amount] = '\0';

    bufferstack.push(text);
    char* undo_info = (char*)malloc((strnlen(text, redactor.getNCol())) * sizeof(char));
    sprintf(undo_info, "13\t%s", text);
    undostack.push(undo_info);

    while (!redostack.empty()) {
        free(redostack.top());
        redostack.pop();
    }
}

void TextEditor::paste(Text& redactor, int currow, int curcol, size_t* buffersize) {
    if (bufferstack.empty()) {
        printf("nothing to paste.\n");
        return;
    }

    char* input = nullptr;

    input = bufferstack.top();
    int text_length = strlen(input);

    if (text_length + strlen(redactor.getArray()[currow]) >= *buffersize) {
        redactor.newbuffer(buffersize);
        redactor.getArray()[currow] = (char*)realloc(redactor.getArray()[currow], *buffersize * sizeof(char));
        if (redactor.getArray()[currow] == nullptr) {
            printf("memory allocation failed");
            exit(1);
        }
    }

    for (int i = strlen(redactor.getArray()[currow]); i >= curcol; i--) {
        redactor.getArray()[currow][i + text_length] = redactor.getArray()[currow][i];
    }

    for (int i = 0; i < text_length; i++) {
        redactor.getArray()[currow][curcol + i] = input[i];
    }

    redactor.setNCol(redactor.getNCol() + text_length);
    char* undo_info = (char*)malloc((strlen(input) + *buffersize) * sizeof(char));
    sprintf(undo_info, "12\t%d\t%d\t%s", currow, curcol, input);
    undostack.push(undo_info);

    while (!redostack.empty()) {
        free(redostack.top());
        redostack.pop();
    }
}

void TextEditor::undo(Text& redactor, size_t bufferSize) {
    if (undostack.empty()) {
        printf("No actions to undo.\n");
        return;
    }

    char* last_action = undostack.top();
    char* last_action_copy = _strdup(last_action);

    redostack.push(last_action);
    undostack.pop();

    int action_type;
    sscanf(last_action, "%d", &action_type);
    int ncol = redactor.getNCol();
    int nrow = redactor.getNrow();
    char** array = redactor.getArray();

    if (action_type == 1) {
        // Undo append
        char* text = strchr(last_action_copy, '\t') + 1;
        size_t len = strlen(text);
        ncol -= len;
        array[nrow][ncol] = '\0';
        redactor.setNCol(ncol);
    }
    else if (action_type == 2) {
        // Undo new line
        nrow--;
        redactor.setNrow(nrow);
        ncol = strlen(array[nrow]);
        redactor.setNCol(ncol);
    }
    else if (action_type == 6) {
        // Undo insert
        int currow, curcol;
        char* token = strtok(last_action_copy, "\t");
        token = strtok(nullptr, "\t");
        currow = atoi(token);
        token = strtok(nullptr, "\t");
        curcol = atoi(token);
        token = strtok(nullptr, "\t");
        char* text = _strdup(token);

        int text_length = strlen(text);
        int new_length = (int)strlen(array[currow]) - text_length;

        for (int i = curcol; i <= new_length; ++i) {
            array[currow][i] = array[currow][i + text_length];
        }
        array[currow][new_length] = '\0';
        redactor.setNCol(strnlen(array[currow], bufferSize));
    }
    else if (action_type == 12) {
        // Undo paste
        int currow, curcol;
        char* token = strtok(last_action_copy, "\t");
        token = strtok(nullptr, "\t");
        currow = atoi(token);
        token = strtok(nullptr, "\t");
        curcol = atoi(token);
        token = strtok(nullptr, "\t");
        char* text = _strdup(token);

        int text_length = strlen(text);
        int new_length = (int)strlen(array[currow]) - text_length;

        for (int i = curcol; i <= new_length; ++i) {
            array[currow][i] = array[currow][i + text_length];
        }
        array[currow][new_length] = '\0';
        redactor.setNCol(strnlen(array[currow], bufferSize));

        free(text);
    }
    else if (action_type == 8) {
        // Undo delete
        int currow, curcol;
        char* token = strtok(last_action_copy, "\t");
        token = strtok(nullptr, "\t");
        currow = atoi(token);
        token = strtok(nullptr, "\t");
        curcol = atoi(token);
        token = strtok(nullptr, "\t");
        char* deleted_text = _strdup(token);

        int deleted_text_length = strlen(deleted_text);
        int new_length = (int)strlen(array[currow]) + deleted_text_length;

        if (new_length >= bufferSize) {
            redactor.newbuffer(&bufferSize);
            array[currow] = (char*)realloc(array[currow], bufferSize * sizeof(char));
            if (array[currow] == nullptr) {
                printf("Memory allocation failed");
                exit(1);
            }
        }

        for (int i = strlen(array[currow]); i >= curcol; i--) {
            array[currow][i + deleted_text_length] = array[currow][i];
        }

        for (int i = 0; i < deleted_text_length; i++) {
            array[currow][curcol + i] = deleted_text[i];
        }
        redactor.setNCol(strnlen(array[currow], bufferSize));

        free(deleted_text);
    }
    else if (action_type == 13) {
        // Undo copy
        char* token = strtok(last_action_copy, "\t");
        token = strtok(nullptr, "\t");
        if (token != nullptr) {
            char* text = _strdup(token);
            if (!bufferstack.empty()) {
                bufferundostack.push(bufferstack.top());
                bufferstack.pop();
            }
            free(text);
        }
    }
    else if (action_type == 9) {
        // Undo cut
        redostack.pop();
        undo(redactor, bufferSize);
        undo(redactor, bufferSize);

        char* undo_info = (char*)malloc((2) * sizeof(char));
        sprintf(undo_info, "9\0");
        redostack.push(undo_info);
    }
    if (action_type == 16) {
        // Undo insert_rp
        int currow, curcol;
        char* token = strtok(last_action_copy, "\t");
        token = strtok(nullptr, "\t");
        currow = atoi(token);
        token = strtok(nullptr, "\t");
        curcol = atoi(token);
        token = strtok(nullptr, "\t");
        char* original_text = _strdup(token);

        char* text = _strdup(array[currow]);
        strcpy(array[currow], original_text);

        char* undo_info = (char*)malloc((strlen(original_text) + bufferSize) * sizeof(char));
        sprintf(undo_info, "16\t%d\t%d\t%s", currow, curcol, text);
        redostack.push(undo_info);

        free(original_text);
    }

    free(last_action_copy);
}

void TextEditor::redo(Text& redactor, size_t bufferSize) {
    if (redostack.empty()) {
        printf("No actions to redo.\n");
        return;
    }

    char* last_action = redostack.top();
    redostack.pop();
    undostack.push(last_action);
    char* last_action_copy = _strdup(last_action);

    int action_type;
    sscanf(last_action, "%d", &action_type);

    int ncol = redactor.getNCol();
    int nrow = redactor.getNrow();
    char** array = redactor.getArray();

    if (action_type == 1) {
        // Redo append
        char* text = strchr(last_action, '\t') + 1;
        strcat(array[nrow], text);
        ncol += strlen(text);
        redactor.setNCol(ncol);
    }
    else if (action_type == 2) {
        // Redo new line
        nrow++;
        redactor.setNrow(nrow);
        redactor.setNCol(0);
    }
    else if (action_type == 6) {
        // Redo insert
        int currow, curcol;
        char* token = strtok(last_action_copy, "\t");
        token = strtok(nullptr, "\t");
        currow = atoi(token);
        token = strtok(nullptr, "\t");
        curcol = atoi(token);
        token = strtok(nullptr, "\t");
        char* text = _strdup(token);

        int text_length = strlen(text);
        if (text_length + strlen(array[currow]) >= bufferSize) {
            redactor.newbuffer(&bufferSize);
            array[currow] = (char*)realloc(array[currow], bufferSize * sizeof(char));
            if (array[currow] == nullptr) {
                printf("Memory allocation failed");
                free(text);
                exit(1);
            }
        }

        for (int i = strlen(array[currow]); i >= curcol; i--) {
            array[currow][i + text_length] = array[currow][i];
        }
        for (int i = 0; i < text_length; i++) {
            array[currow][curcol + i] = text[i];
        }

        free(text);
    }
    else if (action_type == 12) {
        // Redo paste
        int currow, curcol;
        char* token = strtok(last_action_copy, "\t");
        token = strtok(nullptr, "\t");
        currow = atoi(token);
        token = strtok(nullptr, "\t");
        curcol = atoi(token);
        token = strtok(nullptr, "\t");
        char* text = _strdup(token);

        int text_length = strlen(text);
        if (text_length + strlen(array[currow]) >= bufferSize) {
            redactor.newbuffer(&bufferSize);
            array[currow] = (char*)realloc(array[currow], bufferSize * sizeof(char));
            if (array[currow] == nullptr) {
                printf("Memory allocation failed");
                free(text);
                exit(1);
            }
        }

        for (int i = strlen(array[currow]); i >= curcol; i--) {
            array[currow][i + text_length] = array[currow][i];
        }
        for (int i = 0; i < text_length; i++) {
            array[currow][curcol + i] = text[i];
        }

        free(text);
    }
    else if (action_type == 8) {
        // Redo delete
        int currow, curcol;
        char* token = strtok(last_action_copy, "\t");
        token = strtok(nullptr, "\t");
        currow = atoi(token);
        token = strtok(nullptr, "\t");
        curcol = atoi(token);
        token = strtok(nullptr, "\t");
        char* text = _strdup(token);

        int text_length = strlen(text);
        int new_length = (int)strlen(array[currow]) - text_length;

        for (int i = curcol; i <= new_length; ++i) {
            array[currow][i] = array[currow][i + text_length];
        }
        array[currow][new_length] = '\0';

        free(text);
    }
    else if (action_type == 13) {
        // Redo copy
        char* token = strtok(last_action_copy, "\t");
        token = strtok(nullptr, "\t");
        if (token != nullptr) {
            char* text = _strdup(token);
            if (!bufferundostack.empty()) {
                bufferstack.push(bufferundostack.top());
                bufferundostack.pop();
            }
            free(text);
        }
    }
    else if (action_type == 9) {
        // Redo cut
        undostack.pop();
        redo(redactor, bufferSize);
        redo(redactor, bufferSize);

        char tex[2] = "9";

        undostack.push(tex);
    }
    else if (action_type == 16) {
        int currow, curcol;
        char* token = strtok(last_action_copy, "\t");
        token = strtok(nullptr, "\t");
        currow = atoi(token);
        token = strtok(nullptr, "\t");
        curcol = atoi(token);
        token = strtok(nullptr, "\t");
        char* original_text = _strdup(token);

        char* text = _strdup(array[currow]);


        char* undo_info = (char*)malloc((strlen(original_text) + bufferSize) * sizeof(char));
        sprintf(undo_info, "16\t%d\t%d\t%s", currow, curcol, text);
        undostack.push(undo_info);

        strcpy(array[currow], original_text);

        free(original_text);
    }
    free(last_action_copy);
}

void TextEditor::help() {
    printf("you open a text redactor with these functions:\n");
    printf("command-'1': append text \n");
    printf("command-'2': start the new line \n");
    printf("command-'3': write your text in file\n");
    printf("command-'4': read text from file\n");
    printf("command-'5': print the current text to console\n");
    printf("command-'6': insert the text by line and symbol index\n");
    printf("command-'7': search\n");
    printf("command-'8': delete\n");
    printf("command-'9': cut\n");
    printf("command-'10': exit\n");
    printf("command-'11': clear console\n");
    printf("command-'12': paste\n");
    printf("command-'13': copy\n");
    printf("command-'14': undo\n");
    printf("command-'15': redo\n");
    printf("command-'16': insert with replacement\n");
    printf("command-'17': encrypt array\n");
    printf("command-'18': decrypt array\n");
    printf("command-'19': encrypt from file\n");
    printf("command-'20': decrypt from file\n\n");

}
