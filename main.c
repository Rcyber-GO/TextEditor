#include "ikiheader.h"

int main() {
    Node* headEditor = NULL; 
    tambahBaris(&headEditor, "Tess");
    tambahBaris(&headEditor, "Tess");
    tambahBaris(&headEditor, "Tess");

    printf("Sedang menjalankan fitur save file...\n");

    if (simpanKeFile(headEditor, "output_editor.txt")) {
        printf("Berhasil! Silahkan cek file 'output_editor.txt'.\n");
    } else {
        printf("Proses penyimpanan gagal.\n");
    }

    // Pembersihan memori
    hapusSemuaMemori(headEditor);
    
    return 0;
}
