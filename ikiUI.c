#include "ikiheader.h"

void menuAwal(TextEditor *ed) {
    char pilihan;
    
    system("cls");
    printf("====== MINI TEXT EDITOR  ======\n\n");
    printf("Apakah ingin membuka file? (Y/N): ");
    scanf(" %c", &pilihan); 
    getchar();

    if (pilihan == 'Y' || pilihan == 'y') {
        printf("Masukkan nama file yang ingin dibuka (contoh: catatan.txt): ");
        scanf("%99s", ed->nama_file);
        getchar();
        
        int status = bukaFile(ed, ed->nama_file);
        
        if (status == 1) {
            printf("\nFile '%s' berhasil di buka\n", ed->nama_file);
            system("pause");
        } else {
            printf("File tidak ditemukan | Membuat dokumen kosong baru\n");
            inisialisasiEditor(ed);
            system("pause");
        }
    } 
    else {
        inisialisasiEditor(ed);
        printf("\nMembuat file baru kosong\n");
        system("pause");
    }
}
