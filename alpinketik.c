#include "alpinheader.h"

#include <windows.h> 
#include <conio.h>

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Fungsi inisialisasi yang udah ada sebelumnya
void inisialisasiEditor(TextEditor *ed) {
    ed->head = (Node*)malloc(sizeof(Node));
    memset(ed->head->info, 0, MAX_KOLOM);
    ed->head->jumlah_karakter = 0;
    ed->head->prev = NULL;
    ed->head->next = NULL;

    ed->tail = ed->head;
    ed->baris_sekarang = ed->head;
    ed->kolom_sekarang = 0;
    ed->jumlah_baris = 1;
}

void jalankanEditor(TextEditor *ed) {
    system("cls");
    
    gotoxy(0, 0);
    printf("=== PROJEK 2: MINI TEXT EDITOR === (Tekan ESC untuk keluar)");

    int posisi_y_layar = 2; 

    while (1) {
        gotoxy(ed->kolom_sekarang, posisi_y_layar);

        int ch = _getch(); 

        if (ch == 27) { // ESC
            break;
        }
        else if (ch == 8) { // Backspace
            if (ed->kolom_sekarang > 0) {
                ed->kolom_sekarang--; 
                ed->baris_sekarang->info[ed->kolom_sekarang] = '\0'; 
                ed->baris_sekarang->jumlah_karakter--;
                
                gotoxy(ed->kolom_sekarang, posisi_y_layar);
                printf(" "); 
            }
        }
        else if (ch >= 32 && ch <= 126) { // Ngetik biasa
            if (ed->kolom_sekarang < MAX_KOLOM - 1) {
                ed->baris_sekarang->info[ed->kolom_sekarang] = ch;
                printf("%c", ch); 
                
                ed->kolom_sekarang++;
                ed->baris_sekarang->jumlah_karakter++;
            }
        }
        
    }
}