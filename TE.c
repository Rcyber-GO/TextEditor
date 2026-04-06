#include "TE.h"
int i;

void inisialisasiEditor(TextEditor *ed) {
    ed->jumlah_baris = 0;
    for (i = 0; i < MAX_BARIS; i++) {
        ed->konten[i][0] = '\0';
    }
}

void tampilkanEditor(TextEditor *ed) {
    system("cls"); // Gunakan "clear" jika di Linux
    printf("=== C-CLI TEXT EDITOR ===\n");
    printf("Ketik 'SAVE' pada baris baru untuk keluar dan simpan.\n");
    printf("--------------------------------------------------\n");
    
    for (i = 0; i < ed->jumlah_baris; i++) {
        printf("%2d | %s\n", i + 1, ed->konten[i]);
    }
}

void tulisTeks(TextEditor *ed) {
    char buffer[MAX_KOLOM];
    
    while (ed->jumlah_baris < MAX_BARIS) {
        printf("%2d | ", ed->jumlah_baris + 1);
        fgets(buffer, MAX_KOLOM, stdin);
        
        // Hilangkan newline dari fgets
        buffer[strcspn(buffer, "\n")] = 0;

        // Cek keyword untuk berhenti
        if (strcmp(buffer, "SAVE") == 0) {
            break;
        }

        // Masukkan ke array 2 dimensi
        strcpy(ed->konten[ed->jumlah_baris], buffer);
        ed->jumlah_baris++;
    }
}

void simpanKeFile(TextEditor *ed) {
    FILE *file = fopen(ed->nama_file, "w");
    if (file == NULL) {
        printf("Gagal menyimpan file!\n");
        return;
    }

    for (i = 0; i < ed->jumlah_baris; i++) {
        fprintf(file, "%s\n", ed->konten[i]);
    }

    fclose(file);
    printf("File Berhasil disimpan dengan nama: %s\n", ed->nama_file);
}
