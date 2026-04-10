<<<<<<<< HEAD:editor.c
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
        // Update ukuran layar setiap kali loop berjalan (beradaptasi jika user me-resize jendela)
        dapatkanUkuranLayar(&layar_kolom, &layar_baris);
        
        tampilkanEditor(ed);
        
        // Asumsi baris + 3 adalah offset untuk header UI Anda
        gotoxy(ed->kolom_sekarang, ed->baris_sekarang + 3); 

        int ch = _getch(); 

        if (ch == 224) { 
            ch = _getch(); 
            if (ch == 72 && ed->baris_sekarang > 0) ed->baris_sekarang--;            // Up
            else if (ch == 80 && ed->baris_sekarang < ed->jumlah_baris - 1) ed->baris_sekarang++; // Down
            else if (ch == 75 && ed->kolom_sekarang > 0) ed->kolom_sekarang--;       // Left
            else if (ch == 77 && ed->kolom_sekarang < (int)strlen(ed->konten[ed->baris_sekarang])) ed->kolom_sekarang++; // Right
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
        // ENTER (13)
        else if (ch == 13) { 
            // Cek batas memori MAX_BARIS, bukan batas layar untuk menyimpan data
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
        // BACA CTRL+X (24)
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
========
>>>>>>>> 58ddb5b4c57a233df185ac3d490544dd8f5f5512:TE.c
