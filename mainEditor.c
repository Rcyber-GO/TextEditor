#include "TE.h"

int main() {
    TextEditor myEditor;
    
    inisialisasiEditor(&myEditor);

    printf("Masukkan nama file format .txt: ");
    scanf("%s", myEditor.nama_file);
    getchar(); // Membersihkan buffer enter

    tampilkanEditor(&myEditor);
    tulisTeks(&myEditor);
    simpanKeFile(&myEditor);

    printf("\nTerima kasih telah menggunakan editor ini!\n");
    return 0;
}
