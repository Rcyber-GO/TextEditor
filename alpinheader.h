#ifndef ALPINHEADER_H
#define ALPINHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 50
#define MAX_BARIS 1000
#define MAX_KOLOM 120

typedef struct Node {
    char info[MAX_KOLOM]; 
    int jumlah_karakter;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
	Node *head;
	Node *tail;
    Node *baris_sekarang;
    int kolom_sekarang;
    int jumlah_baris;
    char nama_file[100];
} TextEditor;

// Deklarasi fungsi ngetik
void inisialisasiEditor(TextEditor *ed);
void jalankanEditor(TextEditor *ed);
void tambahBarisBaru(TextEditor *ed);
void tanganiRipple(TextEditor *ed, int posisi_y_layar);
void tanganiRippleMundur(TextEditor *ed, int posisi_y_layar);
void tanganiBackspaceKiri(TextEditor *ed, int *posisi_y_layar);
void splitBaris(TextEditor *ed, int *posisi_y_layar); 

#endif