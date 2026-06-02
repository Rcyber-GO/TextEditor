#include "ikiheader.h"

Node* buatNodeBaru(const char* isiTeks) {
    Node* nodeBaru = (Node*)malloc(sizeof(Node));
    if (nodeBaru != NULL) {
        strcpy(nodeBaru->teks, isiTeks);
        nodeBaru->next = NULL;
    }
    return nodeBaru;
}

void tambahBaris(Node** head, const char* isiTeks) {
    Node* nodeBaru = buatNodeBaru(isiTeks);
    
    if (*head == NULL) {
        *head = nodeBaru;
        return;
    }
    
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = nodeBaru;
}

int simpanKeFile(Node* head, const char* namaFile) {
    FILE* file = fopen(namaFile, "w");
    
    if (file == NULL) {
        printf("Gagal membuka atau membuat file!\n");
        return 0;
    }
    
    Node* current = head;
    while (current != NULL) {
        fprintf(file, "%s\n", current->teks);
        current = current->next;
    }
    
    fclose(file);
    return 1;
}

void hapusSemuaMemori(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
