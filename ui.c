#include "ui.h"
void setWarna(int warna) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), warna);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void tampilkanEditor(TextEditor *ed) {
int i;
    gotoxy(0, 0);
    setWarna(11); 
    printf("=======================================================  TEKS EDITOR ==================================================\n");
    setWarna(14); 
    printf("              [CTRL+S] Save | [CTRL+O] Open | [CTRL+Z] Undo | [CTRL+Y] Redo | [ESC] Exit | [CTRL+X] Delete              \n");
    setWarna(11);
    printf("=======================================================================================================================\n");
    
    for (i = 0; i < ed->jumlah_baris; i++) {
        setWarna(7);
        printf("%-120s\n", ed->konten[i]); 
    }
    printf("%-120s\n", ""); 
}