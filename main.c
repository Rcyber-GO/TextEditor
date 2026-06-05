#include "alpinheader.h"
#include "ikiheader.h"

int main() {
    TextEditor editor;

    // Jalankan Alur UI Menu Awal
    menuAwal(&editor);

    // Jalankan Editor Ketik
    jalankanEditor(&editor);

    // Alur SAVE Data
    system("cls");
    printf("=== KELUAR DARI EDITOR ===\n");
    printf("Masukkan nama file untuk menyimpan hasil akhir: ");
    scanf("%99s", editor.nama_file);

    // Memanggil fungsi simpanFile
    int status_simpan = simpanFile(&editor, editor.nama_file);
    if (status_simpan == 1) {
        printf("\nFile '%s' Berhasil disimpan\n", editor.nama_file);
    } else {
        printf("\nGagal menyimpan file\n");
    }

    return 0;
}
