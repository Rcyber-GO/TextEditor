#ifndef IKIHEADER_H
#define IKIHEADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char teks[256];
    struct node* next;
} node;

node* buatNodeBaru(const char* isiTeks);
void tambahBaris(node** head, const char* isiTeks);
int simpanKeFile(node* head, const char* namaFile);
void hapusSemuaMemori(node* head);

#endif
