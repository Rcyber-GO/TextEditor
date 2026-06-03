#include <stdio.h>
#include <stdlib.h>
#include "alpinheader.h"
#include "ikiheader.h"

int main() {
    TextEditor editor;
    inisialisasiEditor(&editor);
    jalankanEditor(&editor);
    system("cls"); 
    printf("=== KELUAR DARI EDITOR ===\n");
    printf("Masukkan nama file untuk menyimpan (contoh: tugasku.txt): ");
    scanf("%99s", editor.nama_file); 
    int status = simpanFile(&editor, editor.nama_file);
    if (status == 1) {
        printf("\n[SUCCESS] File '%s' berhasil disimpan!\n", editor.nama_file);
    } else {
        printf("\n[ERROR] Gagal menyimpan file!\n");
    }
    
    return 0;
}
