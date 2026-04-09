#include "TE.h"

TextEditor historyStack[MAX_HISTORY];
int current_history_idx = -1;
int max_history_idx = -1;

void resetHistory() {
    current_history_idx = -1;
    max_history_idx = -1;
}

void simpanState(TextEditor *ed) {
    if (current_history_idx < MAX_HISTORY - 1) {
        current_history_idx++;
    } else {
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            historyStack[i] = historyStack[i + 1];
        }
    }
    historyStack[current_history_idx] = *ed; 
    max_history_idx = current_history_idx;  
}

void undo(TextEditor *ed) {
    if (current_history_idx > 0) {
        current_history_idx--;
        *ed = historyStack[current_history_idx];
    }
}

void redo(TextEditor *ed) {
    if (current_history_idx < max_history_idx) {
        current_history_idx++;
        *ed = historyStack[current_history_idx];
    }
}

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
    
    resetHistory();
    simpanState(ed);
}

void tampilkanEditor(TextEditor *ed) {
    gotoxy(0, 0);
    setWarna(11); 
    printf("=======================================================  TEKS EDITOR ==================================================\n");
    setWarna(14); 
    printf("   [CTRL+S] Simpan | [CTRL+O] Buka | [CTRL+A] Block | [CTRL+Z] Undo | [CTRL+Y] Redo | [ESC] Keluar | [CTRL+X] Delete \n");
    setWarna(11);
    printf("=======================================================================================================================\n");
    
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

        if (ch == 224) { 
            ch = _getch(); 
            if (ch == 72 && ed->baris_sekarang > 0) ed->baris_sekarang--;            // Up
            else if (ch == 80 && ed->baris_sekarang < ed->jumlah_baris - 1) ed->baris_sekarang++; // Down
            else if (ch == 75 && ed->kolom_sekarang > 0) ed->kolom_sekarang--;       // Left
            else if (ch == 77 && ed->kolom_sekarang < (int)strlen(ed->konten[ed->baris_sekarang])) ed->kolom_sekarang++; // Right
        } 
        // BACA CTRL+A (1) - BLOCK ALL
        else if (ch == 1) {
            ed->is_blocked = 1;
        }
        // BACA CTRL+Z (26) - UNDO
        else if (ch == 26) {
            undo(ed);
        }
        // BACA CTRL+Y (25) - REDO
        else if (ch == 25) {
            redo(ed);
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
        
        else if (ch == 13) { 
            if (ed->jumlah_baris < MAX_BARIS) {
                for (i = ed->jumlah_baris; i > ed->baris_sekarang + 1; i--) {
                    strcpy(ed->konten[i], ed->konten[i-1]);
                }
                ed->jumlah_baris++;
                
                strcpy(ed->konten[ed->baris_sekarang + 1], &ed->konten[ed->baris_sekarang][ed->kolom_sekarang]);
                memset(&ed->konten[ed->baris_sekarang][ed->kolom_sekarang], 0, MAX_KOLOM - ed->kolom_sekarang);
                
                ed->baris_sekarang++;
                ed->kolom_sekarang = 0;
                
                if (!_kbhit()) simpanState(ed);
            }
        } 
        // BACKSPACE (8)
        else if (ch == 8) { 
            int changed = 0;
            if (ed->kolom_sekarang > 0) {
                int len = strlen(ed->konten[ed->baris_sekarang]);
                for (i = ed->kolom_sekarang - 1; i < len; i++) {
                    ed->konten[ed->baris_sekarang][i] = ed->konten[ed->baris_sekarang][i+1];
                }
                ed->kolom_sekarang--;
                changed = 1;
            } 
            else if (ed->baris_sekarang > 0) {
                int panjang_baris_atas = strlen(ed->konten[ed->baris_sekarang - 1]);
                int panjang_baris_ini = strlen(ed->konten[ed->baris_sekarang]);

                if (panjang_baris_atas + panjang_baris_ini < MAX_KOLOM) {
                    strcat(ed->konten[ed->baris_sekarang - 1], ed->konten[ed->baris_sekarang]);
                    for (i = ed->baris_sekarang; i < ed->jumlah_baris - 1; i++) {
                        strcpy(ed->konten[i], ed->konten[i+1]);
                    }
                    memset(ed->konten[ed->jumlah_baris - 1], 0, MAX_KOLOM);
                    ed->jumlah_baris--;
                    ed->baris_sekarang--;
                    ed->kolom_sekarang = panjang_baris_atas;
                    changed = 1;
                }
            }
            if (changed && !_kbhit()) simpanState(ed);
        }
        // KARAKTER BIASA
        else if (ch >= 32 && ch <= 126) { 
            int len = strlen(ed->konten[ed->baris_sekarang]);

            // AUTO-WRAP
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
                    continue; 
                }
            }

            if (len >= MAX_KOLOM - 2) {
                continue; 
            }

        
            for (i = len; i >= ed->kolom_sekarang; i--) {
                ed->konten[ed->baris_sekarang][i+1] = ed->konten[ed->baris_sekarang][i];
            }
            ed->konten[ed->baris_sekarang][ed->kolom_sekarang] = (char)ch; 
            ed->kolom_sekarang++;
            
            if (!_kbhit()) {
                simpanState(ed);
            }
        }
        // BACA CTRL+X (24) - HAPUS 1 KATA KE KIRI
        else if (ch == 24) {
            int changed = 0;
            
            if (ed->kolom_sekarang > 0) {
                int hapus_sampai = ed->kolom_sekarang - 1;
                
                while (hapus_sampai >= 0 && ed->konten[ed->baris_sekarang][hapus_sampai] == ' ') {
                    hapus_sampai--;
                }
                
                while (hapus_sampai >= 0 && ed->konten[ed->baris_sekarang][hapus_sampai] != ' ') {
                    hapus_sampai--;
                }
                
                int posisi_baru = hapus_sampai + 1;
                int jumlah_dihapus = ed->kolom_sekarang - posisi_baru;
                int len = strlen(ed->konten[ed->baris_sekarang]);
                
                for (i = posisi_baru; i <= len - jumlah_dihapus; i++) {
                    ed->konten[ed->baris_sekarang][i] = ed->konten[ed->baris_sekarang][i + jumlah_dihapus];
                }
                
                ed->kolom_sekarang = posisi_baru;
                changed = 1;
            } 
            else if (ed->baris_sekarang > 0) {
                int panjang_baris_atas = strlen(ed->konten[ed->baris_sekarang - 1]);
                int panjang_baris_ini = strlen(ed->konten[ed->baris_sekarang]);

                if (panjang_baris_atas + panjang_baris_ini < MAX_KOLOM) {
                    strcat(ed->konten[ed->baris_sekarang - 1], ed->konten[ed->baris_sekarang]);
                    for (i = ed->baris_sekarang; i < ed->jumlah_baris - 1; i++) {
                        strcpy(ed->konten[i], ed->konten[i+1]);
                    }
                    memset(ed->konten[ed->jumlah_baris - 1], 0, MAX_KOLOM);
                    ed->jumlah_baris--;
                    ed->baris_sekarang--;
                    ed->kolom_sekarang = panjang_baris_atas;
                    changed = 1;
                }
            }

            if (changed && !_kbhit()) simpanState(ed);
        }
    }
}

// Implementasi Fungsi Open File
void bukaFile(TextEditor *ed) {
    setWarna(14); 
    printf("Masukkan nama file yang ingin dibuka (.txt): ");
    setWarna(7);
    scanf("%s", ed->nama_file);

    FILE *file = fopen(ed->nama_file, "r");
    if (file == NULL) {
        setWarna(12); 
        printf("\n[ERROR] File tidak ditemukan atau gagal dibuka!\n");
        setWarna(7);
        Sleep(2000);
        return;
    }

    inisialisasiEditor(ed); 

    // Membaca file baris demi baris
    ed->jumlah_baris = 0;
    while (fgets(ed->konten[ed->jumlah_baris], MAX_KOLOM, file)) {
        ed->konten[ed->jumlah_baris][strcspn(ed->konten[ed->jumlah_baris], "\n")] = 0;
        ed->jumlah_baris++;
        if (ed->jumlah_baris >= MAX_BARIS) break;
    }

    if (ed->jumlah_baris == 0) ed->jumlah_baris = 1; 

    ed->baris_sekarang = ed->jumlah_baris - 1;
    ed->kolom_sekarang = strlen(ed->konten[ed->baris_sekarang]);

    fclose(file);
    
    // Simpan history setelah memuat file
    resetHistory();
    simpanState(ed);

    setWarna(10); 
    printf("\n[SUCCESS] File berhasil dimuat: %s\n", ed->nama_file);
    setWarna(7);
    Sleep(2000);
}

void simpanKeFile(TextEditor *ed) {
    setWarna(14);
    printf("Masukkan nama file dengan format .txt: ");
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