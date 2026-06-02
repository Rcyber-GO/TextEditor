#ifndef IKIHEADER_H
#define IKIHEADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char teks[256];
    struct Node* next;
} Node;

Node* buatNodeBaru(const char* isiTeks);
void tambahBaris(Node** head, const char* isiTeks);
int simpanKeFile(Node* head, const char* namaFile);
void hapusSemuaMemori(Node* head);

#endif
