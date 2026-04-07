#include "TE.h"

int main() {
    // Mengubah judul jendela Console menggunakan fitur dari windows.h
    SetConsoleTitle("Dev-C++ CLI Text Editor");
    
    TextEditor myEditor;
    
    inisialisasiEditor(&myEditor);

    setWarna(11);
    printf("==========================================\n");
    printf("              SELAMAT MENGETIK            \n");
    printf("==========================================\n");
    setWarna(7);
    
    tampilkanEditor(&myEditor);
	system("cls"); 
    tulisTeks(&myEditor);

    setWarna(11);
    printf("\nTerima kasih telah menggunakan editor ini!\n");
    setWarna(7);
    
    system("pause");
    return 0;
}
