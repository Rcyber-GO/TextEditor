#include "file_io.h"
#include "ui.h"
#include "history.h"
#include "editor.h"
void bukaFile(TextEditor *ed) {
    setWarna(14); 
    printf("Masukkan nama file yang ingin dibuka (.txt): ");
    setWarna(7);
    scanf("%s", ed->nama_file);

    FILE *file = fopen(ed->nama_file, "r");
    if (file == NULL) {
        setWarna(12); 
        printf("\n[ERROR] File tidak ditemukan atau gagal dibuka!\n");
        setWarna(7);
        Sleep(2000);
        return;
    }

    inisialisasiEditor(ed); 

    // Membaca file baris demi baris
    ed->jumlah_baris = 0;
    while (fgets(ed->konten[ed->jumlah_baris], MAX_KOLOM, file)) {
        ed->konten[ed->jumlah_baris][strcspn(ed->konten[ed->jumlah_baris], "\n")] = 0;
        ed->jumlah_baris++;
        if (ed->jumlah_baris >= MAX_BARIS) break;
    }

    if (ed->jumlah_baris == 0) ed->jumlah_baris = 1; 

    ed->baris_sekarang = ed->jumlah_baris - 1;
    ed->kolom_sekarang = strlen(ed->konten[ed->baris_sekarang]);

    fclose(file);
    
    // Simpan history setelah memuat file
    resetHistory();
    simpanState(ed);

    setWarna(10); 
    printf("\n[SUCCESS] File berhasil dimuat: %s\n", ed->nama_file);
    setWarna(7);
    Sleep(2000);
}

void simpanKeFile(TextEditor *ed) {
int i;
    setWarna(14);
    printf("Save file dengan format .txt: ");
    setWarna(7);
    scanf("%s", ed->nama_file);

    FILE *file = fopen(ed->nama_file, "w");
    if (file == NULL) {
        setWarna(12);
        printf("\n[ERROR] Gagal membuat file!\n");
        setWarna(7);
        Sleep(2000);
        return;
    }

    for (i = 0; i < ed->jumlah_baris; i++) {
        fprintf(file, "%s\n", ed->konten[i]);
    }

    fclose(file);
    setWarna(10); 
    printf("\n[SUCCESS] File berhasil disimpan: %s\n", ed->nama_file);
    setWarna(7);
    Sleep(2000);
}
