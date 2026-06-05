#include "ikiheader.h"

int bukaFile(TextEditor *ed, const char *nama_file) {
    FILE *file = fopen(nama_file, "r");
    if (file == NULL) {
        return 0;
    }

    char buffer[MAX_KOLOM];
    Node *bantu_sekarang = NULL;
    int hitung_baris = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        Node *baru = (Node*)malloc(sizeof(Node));
        strcpy(baru->info, buffer);
        baru->jumlah_karakter = strlen(buffer);
        baru->next = NULL;
        baru->prev = NULL;

        if (hitung_baris == 0) {
            ed->head = baru;
            bantu_sekarang = baru;
        } else {
            bantu_sekarang->next = baru;
            baru->prev = bantu_sekarang;
            bantu_sekarang = baru;
        }
        hitung_baris++;
    }
    fclose(file);

    if (hitung_baris == 0) {
        return 0; 
    }
    ed->tail = bantu_sekarang;
    ed->baris_sekarang = ed->head; // mulai dari baris pertama
    ed->kolom_sekarang = 0;        // mulai dari kolom 0
    ed->jumlah_baris = hitung_baris;
    strcpy(ed->nama_file, nama_file);

    return 1;
}
