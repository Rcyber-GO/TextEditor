#ifndef HISTORY_H
#define HISTORY_H

#include "TE.h"

void resetHistory();
void simpanState(TextEditor *ed);
void undo(TextEditor *ed);
void redo(TextEditor *ed);

#endif