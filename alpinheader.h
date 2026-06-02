#ifndef ALPINHEADER_H
#define ALPINHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#endif
