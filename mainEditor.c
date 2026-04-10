#include "TE.h"
#include "editor.h"
#include "ui.h"

int main() {
    // Mengatur judul window console
    SetConsoleTitle("Text Editor");
    
    TextEditor myEditor;
    
    // Inisialisasi state awal editor
    inisialisasiEditor(&myEditor);
    
    // Masuk ke loop utama editor
    tulisTeks(&myEditor);

    // Pesan keluar setelah menekan ESC
    setWarna(11);
    printf("\nTerima kasih telah menggunakan editor ini\n");
    setWarna(7); // Kembalikan ke warna default
    
    system("pause");
    return 0;
}