#ifndef TE_H
#define TE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#define MAX_HISTORY 50
#define MAX_BARIS 1000
#define MAX_KOLOM 120

// Struktur TextEditor
typedef struct {
    char konten[MAX_BARIS][MAX_KOLOM];
    int baris_sekarang;
    int kolom_sekarang;
    int jumlah_baris;
    char nama_file[100];
} TextEditor;

#endif