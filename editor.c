#include "editor.h"
#include "ui.h"
#include "history.h"
#include "file_io.h"

void dapatkanUkuranLayar(int *kolom, int *baris) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *kolom = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *baris = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void inisialisasiEditor(TextEditor *ed) {
    memset(ed->konten, 0, sizeof(ed->konten));
    ed->baris_sekarang = 0;
    ed->kolom_sekarang = 0;
    ed->jumlah_baris = 1; 
    
    resetHistory();
    simpanState(ed);
}

void tulisTeks(TextEditor *ed) {
    int i;
    int layar_kolom, layar_baris;

    while (1) {
        dapatkanUkuranLayar(&layar_kolom, &layar_baris);
        tampilkanEditor(ed);
        gotoxy(ed->kolom_sekarang, ed->baris_sekarang + 3); 

        int ch = _getch(); 

        if (ch == 224) { 
            ch = _getch(); 
            if (ch == 72 && ed->baris_sekarang > 0) {
                ed->baris_sekarang--;
                if (ed->kolom_sekarang > (int)strlen(ed->konten[ed->baris_sekarang])) {
                    ed->kolom_sekarang = strlen(ed->konten[ed->baris_sekarang]);
                }
            }
            else if (ch == 80 && ed->baris_sekarang < ed->jumlah_baris - 1) {
                ed->baris_sekarang++;
                if (ed->kolom_sekarang > (int)strlen(ed->konten[ed->baris_sekarang])) {
                    ed->kolom_sekarang = strlen(ed->konten[ed->baris_sekarang]);
                }
            }
            else if (ch == 75 && ed->kolom_sekarang > 0) {
                ed->kolom_sekarang--;
            }
            else if (ch == 77 && ed->kolom_sekarang < (int)strlen(ed->konten[ed->baris_sekarang])) {
                ed->kolom_sekarang++;
            }
        } 
        else if (ch == 26) undo(ed);
        else if (ch == 25) redo(ed);
        else if (ch == 27) { system("cls"); break; } 
        else if (ch == 19) { system("cls"); simpanKeFile(ed); system("cls"); } 
        else if (ch == 15) { system("cls"); bukaFile(ed); system("cls"); }
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
        else if (ch >= 32 && ch <= 126) { 
            int len = strlen(ed->konten[ed->baris_sekarang]);

            // AUTO-WRAP Logic
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
                } else continue;
            }

            // RIPPLE EFFECT Logic (Simplified and Fixed)
            if (len >= MAX_KOLOM - 2) {
                int b = ed->baris_sekarang;
                char char_overflow;
                
                while (b < MAX_BARIS - 1 && strlen(ed->konten[b]) >= MAX_KOLOM - 2) {
                    int len_current = strlen(ed->konten[b]);
                    char_overflow = ed->konten[b][len_current - 1];
                    ed->konten[b][len_current - 1] = '\0';
                    
                    b++;
                    if (b == ed->jumlah_baris) {
                        ed->jumlah_baris++;
                        memset(ed->konten[b], 0, MAX_KOLOM);
                    }
                    
                    int len_next = strlen(ed->konten[b]);
                    int j;
                    for (j = len_next; j >= 0; j--) {
                        ed->konten[b][j + 1] = ed->konten[b][j];
                    }
                    ed->konten[b][0] = char_overflow;
                }
                len = strlen(ed->konten[ed->baris_sekarang]);
            }

            // INSERT CHARACTER
            for (i = len; i >= ed->kolom_sekarang; i--) {
                ed->konten[ed->baris_sekarang][i+1] = ed->konten[ed->baris_sekarang][i];
            }
            ed->konten[ed->baris_sekarang][ed->kolom_sekarang] = (char)ch; 
            ed->kolom_sekarang++;
            
            if (!_kbhit()) simpanState(ed);
        }
        else if (ch == 24) { // CTRL+X
            int changed = 0;
            if (ed->kolom_sekarang > 0) {
                int hapus_sampai = ed->kolom_sekarang - 1;
                while (hapus_sampai >= 0 && ed->konten[ed->baris_sekarang][hapus_sampai] == ' ') hapus_sampai--;
                while (hapus_sampai >= 0 && ed->konten[ed->baris_sekarang][hapus_sampai] != ' ') hapus_sampai--;
                
                int posisi_baru = hapus_sampai + 1;
                int jumlah_dihapus = ed->kolom_sekarang - posisi_baru;
                int len = strlen(ed->konten[ed->baris_sekarang]);
                
                for (i = posisi_baru; i <= len - jumlah_dihapus; i++) {
                    ed->konten[ed->baris_sekarang][i] = ed->konten[ed->baris_sekarang][i + jumlah_dihapus];
                }
                ed->kolom_sekarang = posisi_baru;
                changed = 1;
            }
            if (changed && !_kbhit()) simpanState(ed);
        }
    }
}
