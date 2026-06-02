#include "ikiheader.h"

node* buatNodeBaru(const char* isiTeks) {
    node* nodeBaru = (node*)malloc(sizeof(node));
    if (nodeBaru != NULL) {
        strcpy(nodeBaru->teks, isiTeks);
        nodeBaru->next = NULL;
    }
    return nodeBaru;
}

void tambahBaris(node** head, const char* isiTeks) {
    node* nodeBaru = buatNodeBaru(isiTeks);
    
    if (*head == NULL) {
        *head = nodeBaru;
        return;
    }
    
    node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = nodeBaru;
}

int simpanKeFile(node* head, const char* namaFile) {
    FILE* file = fopen(namaFile, "w");
    
    if (file == NULL) {
        printf("Gagal membuka atau membuat file!\n");
        return 0;
    }
    
    node* current = head;
    while (current != NULL) {
        fprintf(file, "%s\n", current->teks);
        current = current->next;
    }
    
    fclose(file);
    return 1;
}

void hapusSemuaMemori(node* head) {
    node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
