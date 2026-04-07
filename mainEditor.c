#include "TE.h"

int main() {
    // Mengubah judul jendela Console menggunakan fitur dari windows.h
    SetConsoleTitle("Dev-C++ CLI Text Editor");
    
    TextEditor myEditor;
    
    inisialisasiEditor(&myEditor);

    setWarna(11);
    printf("==========================================\n");
    printf("  SELAMAT DATANG DI APLIKASI TEXT EDITOR  \n");
    printf("==========================================\n");
    setWarna(7);
    
    printf("Masukkan nama file format .txt: ");
    scanf("%s", myEditor.nama_file);
    getchar(); // Membersihkan buffer newline agar fgets selanjutnya tidak terlewat

    // Memanggil fungsi baru untuk memeriksa/memuat file eksisting
    bukaFile(&myEditor);

    // Proses inti
    tampilkanEditor(&myEditor);
    tulisTeks(&myEditor);
    simpanKeFile(&myEditor);

    setWarna(11);
    printf("\nTerima kasih telah menggunakan editor ini!\n");
    setWarna(7);
    
    system("pause");
    return 0;
}
