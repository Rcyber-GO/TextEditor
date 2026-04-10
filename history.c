#include "history.h"

TextEditor historyStack[MAX_HISTORY];
int current_history_idx = -1;
int max_history_idx = -1;
void resetHistory() {
    current_history_idx = -1;
    max_history_idx = -1;
}

void simpanState(TextEditor *ed) {
int i;
    if (current_history_idx < MAX_HISTORY - 1) {
        current_history_idx++;
    } else {
        for (i = 0; i < MAX_HISTORY - 1; i++) {
            historyStack[i] = historyStack[i + 1];
        }
    }
    historyStack[current_history_idx] = *ed; 
    max_history_idx = current_history_idx;  
}

void undo(TextEditor *ed) {
    if (current_history_idx > 0) {
        current_history_idx--;
        *ed = historyStack[current_history_idx];
    }
}

void redo(TextEditor *ed) {
    if (current_history_idx < max_history_idx) {
        current_history_idx++;
        *ed = historyStack[current_history_idx];
    }
}
