#include "TE.h"

// Fungsi untuk mengubah warna teks di konsol Windows
void setWarna(int warna) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), warna);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int i;

void inisialisasiEditor(TextEditor *ed) {
    memset(ed->konten, 0, sizeof(ed->konten));
    ed->baris_sekarang = 0;
    ed->kolom_sekarang = 0;
    ed->jumlah_baris = 1; 
    ed->is_blocked = 0;
}

void tampilkanEditor(TextEditor *ed) {
    gotoxy(0, 0);
    setWarna(11); 
    printf("=======================  TEKS EDITOR ======================\n");
    setWarna(14); 
    printf("     Tekan [CTRL+S] Simpan | [ESC] Keluar Tanpa Simpan     \n");
    setWarna(11);
    printf("===========================================================\n");
    
    for (i = 0; i < ed->jumlah_baris; i++) {
        setWarna(7);
        printf("%-120s\n", ed->konten[i]); 
    }
    printf("%-120s\n", ""); 
}

void tulisTeks(TextEditor *ed) {
    while (1) {
        tampilkanEditor(ed);
        
        gotoxy(ed->kolom_sekarang, ed->baris_sekarang + 3); 

        int ch = _getch(); 

        if (ch != 1 && ed->is_blocked) {
            ed->is_blocked = 0;
            tampilkanEditor(ed);
        }

        // DETEKSI TOMBOL ARROW (224)
        if (ch == 224) { 
            ch = _getch(); 
            if (ch == 72 && ed->baris_sekarang > 0) ed->baris_sekarang--;           // Up
            else if (ch == 80 && ed->baris_sekarang < ed->jumlah_baris - 1) ed->baris_sekarang++; // Down
            else if (ch == 75 && ed->kolom_sekarang > 0) ed->kolom_sekarang--;      // Left
            else if (ch == 77 && ed->kolom_sekarang < (int)strlen(ed->konten[ed->baris_sekarang])) ed->kolom_sekarang++; // Right
        } 
        // BACA CTRL+A (1) - BLOCK ALL
        else if (ch == 1) {
            ed->is_blocked = 1;
        }
        // BACA ESC (27) - KELUAR
        else if (ch == 27) { 
            system("cls");
            break; 
        } 
        // BACA CTRL+S (19) - SIMPAN
        else if (ch == 19) { 
            system("cls");
            simpanKeFile(ed); 
            system("cls");
        } 
        // BACA CTRL+O (15) - OPEN FILE
        else if (ch == 15) { 
            system("cls");
            bukaFile(ed);
            system("cls");
        }
        // ENTER (13) - Paragraf Baru
        else if (ch == 13) { 
            if (ed->jumlah_baris < MAX_BARIS) {
                // Geser baris-baris di bawahnya ke bawah
                for (i = ed->jumlah_baris; i > ed->baris_sekarang + 1; i--) {
                    strcpy(ed->konten[i], ed->konten[i-1]);
                }
                ed->jumlah_baris++;
                
                // Pindahkan sisa teks di kanan kursor ke baris baru
                strcpy(ed->konten[ed->baris_sekarang + 1], &ed->konten[ed->baris_sekarang][ed->kolom_sekarang]);
                memset(&ed->konten[ed->baris_sekarang][ed->kolom_sekarang], 0, MAX_KOLOM - ed->kolom_sekarang);
                
                ed->baris_sekarang++;
                ed->kolom_sekarang = 0;
            }
        } 
        // BACKSPACE (8)
        else if (ch == 8) { 
            if (ed->kolom_sekarang > 0) {
                // Kondisi 1: Menghapus karakter di baris yang sama
                int len = strlen(ed->konten[ed->baris_sekarang]);
                for (i = ed->kolom_sekarang - 1; i < len; i++) {
                    ed->konten[ed->baris_sekarang][i] = ed->konten[ed->baris_sekarang][i+1];
                }
                ed->kolom_sekarang--;
            }
        }   
        // KARAKTER BIASA (Spasi sampai ~)
        else if (ch >= 32 && ch <= 126) { 
            // LOGIKA AUTO-WRAP (Pindah baris otomatis jika kolom penuh)
            if (ed->kolom_sekarang >= MAX_KOLOM - 2) { 
                if (ed->jumlah_baris < MAX_BARIS) {
                    for (i = ed->jumlah_baris; i > ed->baris_sekarang + 1; i--) {
                        strcpy(ed->konten[i], ed->konten[i-1]);
                    }
                    ed->jumlah_baris++;
                    ed->baris_sekarang++;    
                    ed->kolom_sekarang = 0;   
                    memset(ed->konten[ed->baris_sekarang], 0, MAX_KOLOM);
                    len = 0;
                } else {
                    continue; // Jika baris sudah maksimal stop
                }
            }

            // Insert karakter ke posisi kursor
            int len = strlen(ed->konten[ed->baris_sekarang]);
            for (i = len; i >= ed->kolom_sekarang; i--) {
                ed->konten[ed->baris_sekarang][i+1] = ed->konten[ed->baris_sekarang][i];
            }
            ed->konten[ed->baris_sekarang][ed->kolom_sekarang] = (char)ch; 
            ed->kolom_sekarang++;
        }
    }
}


void simpanKeFile(TextEditor *ed) {
    setWarna(14); // Kuning
    printf("Masukkan nama file (contoh: catatan.txt): ");
    setWarna(7);
    scanf("%s", ed->nama_file);

    FILE *file = fopen(ed->nama_file, "w");
    if (file == NULL) {
        setWarna(12); 
        printf("\n[ERROR] Gagal membuat file!\n");
        setWarna(7);
        Sleep(2000);
        return;
    }

    for (i = 0; i < ed->jumlah_baris; i++) {
        fprintf(file, "%s\n", ed->konten[i]);
    }

    fclose(file);
    setWarna(10); 
    printf("\n[SUCCESS] File berhasil disimpan: %s\n", ed->nama_file);
    setWarna(7);
    Sleep(2000);
}
