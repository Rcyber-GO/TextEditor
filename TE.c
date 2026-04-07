#include "TE.h"

// Fungsi untuk mengubah warna teks di konsol Windows
void setWarna(int warna) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), warna);
}
int i;
void inisialisasiEditor(TextEditor *ed) {
    ed->jumlah_baris = 0;
    for (i = 0; i < MAX_BARIS; i++) {
        ed->konten[i][0] = '\0';
    }
}

// Pengembangan: Membaca isi file jika file tersebut sudah ada
void bukaFile(TextEditor *ed) {
    FILE *file = fopen(ed->nama_file, "r");
    if (file != NULL) {
        // File sudah ada, baca isinya dan masukkan ke array 2 dimensi
        while (fgets(ed->konten[ed->jumlah_baris], MAX_KOLOM, file) != NULL && ed->jumlah_baris < MAX_BARIS) {
            // Hilangkan karakter newline di akhir setiap baris
            ed->konten[ed->jumlah_baris][strcspn(ed->konten[ed->jumlah_baris], "\n")] = 0; 
            ed->jumlah_baris++;
        }
        fclose(file);
        setWarna(10); // Warna Hijau Terang
        printf("File ditemukan! Memuat isi file ke dalam editor...\n");
        setWarna(7);  // Kembali ke warna default
    } else {
        setWarna(14); // Warna Kuning
        printf("File belum ada. File baru akan dibuat saat Anda menyimpan.\n");
        setWarna(7);
    }
    Sleep(1500); // Efek jeda (dari windows.h) sebelum masuk ke editor
}

void tampilkanEditor(TextEditor *ed) {
    system("cls");
    setWarna(11); // Warna Cyan
    printf("==================================================\n");
    printf("                   TEXT EDITOR                    \n");
    printf("==================================================\n");
    
    setWarna(14); // Warna Kuning
    printf("File Saat Ini: %s\n", ed->nama_file);
    printf("Ketik 'SAVE' pada baris baru untuk keluar dan simpan.\n");
    printf("--------------------------------------------------\n");
    setWarna(7); // Warna putih/abu-abu standar
    
    // Tampilkan isi teks yang sudah ada (jika tadi memuat file)
    for (i = 0; i < ed->jumlah_baris; i++) {
        printf("%2d | %s\n", i + 1, ed->konten[i]);
    }
}

void tulisTeks(TextEditor *ed) {
    char buffer[MAX_KOLOM];
    
    while (ed->jumlah_baris < MAX_BARIS) {
        setWarna(11); // Cyan untuk nomor baris
        printf("%2d | ", ed->jumlah_baris + 1); 
        setWarna(7);  // Putih untuk ketikan pengguna
        
        fgets(buffer, MAX_KOLOM, stdin);
        
        // Hilangkan newline dari fgets
        buffer[strcspn(buffer, "\n")] = 0;

        // Cek keyword untuk berhenti
        if (strcmp(buffer, "SAVE") == 0) {
            break;
        }

        // Masukkan ke array 2 dimensi
        strcpy(ed->konten[ed->jumlah_baris], buffer);
        ed->jumlah_baris++;
    }
    
    // Validasi jika kapasitas array penuh
    if (ed->jumlah_baris >= MAX_BARIS) {
        setWarna(12); // Merah
        printf("\n[!] Batas maksimal baris (%d) telah tercapai.\n", MAX_BARIS);
        setWarna(7);
        system("pause");
    }
}

void simpanKeFile(TextEditor *ed) {
	
    FILE *file = fopen(ed->nama_file, "w");
    if (file == NULL) {
        setWarna(12); // Merah
        printf("\nGagal menyimpan file ke local disk!\n");
        setWarna(7);
        return;
    }

    for (i = 0; i < ed->jumlah_baris; i++) {
        fprintf(file, "%s\n", ed->konten[i]);
    }

    fclose(file);
    setWarna(10); // Hijau
    printf("\n[SUCCESS] File berhasil disimpan pada local host dengan nama: %s\n", ed->nama_file);
    setWarna(7);
}
