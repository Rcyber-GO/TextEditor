#include "TE.h"

int main() {
    SetConsoleTitle("Text Editor");
    TextEditor myEditor;
    inisialisasiEditor(&myEditor);
    tampilkanEditor(&myEditor);
    system("cls"); 
    tulisTeks(&myEditor);

    setWarna(11);
    printf("\nTerima kasih telah menggunakan editor ini\n");
    setWarna(7);
    
    system("pause");
    return 0;
}