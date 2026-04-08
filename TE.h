#ifndef TE_H
#define TE_H
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#define MAX_BARIS 1000
#define MAX_KOLOM 120

typedef struct {
    char konten[MAX_BARIS][MAX_KOLOM];
    int baris_sekarang;
    int kolom_sekarang; 
    int jumlah_baris;
    char nama_file[50];
    int is_blocked;
} TextEditor;

void inisialisasiEditor(TextEditor *ed);
void setWarna(int warna); 
void gotoxy(int x, int y);
void tampilkanEditor(TextEditor *ed);
void tulisTeks(TextEditor *ed);
void simpanKeFile(TextEditor *ed);
void bukaFile(TextEditor *ed);

#endif
