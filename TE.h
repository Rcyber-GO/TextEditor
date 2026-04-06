#ifndef TE_H
#define TE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BARIS 100
#define MAX_KOLOM 150

typedef struct {
    char konten[MAX_BARIS][MAX_KOLOM];
    int jumlah_baris;
    char nama_file[50];
} TextEditor;

// Prototipe Fungsi
void inisialisasiEditor(TextEditor *ed);
void tampilkanEditor(TextEditor *ed);
void tulisTeks(TextEditor *ed);
void simpanKeFile(TextEditor *ed);

#endif
