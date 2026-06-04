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

void tambahBarisBaru(TextEditor *ed) {
    // Disini saya mengalokasikan memori untuk membuat sebuah node baru
    Node *node_baru = (Node*)malloc(sizeof(Node));
    memset(node_baru->info, 0, MAX_KOLOM);
    
    
    // Proses menyambungkan node yg baru dibuat ke node sebelumnya (prev) dan ke node yg ada di depannya(next). 
    // Logika ini digunakan untuk proses insert baris ditengah tengah baris supaya baris yg baru bisa nyambung ke baris sebelum dan setelahnya 
    // Logikanya prev dari node baru ini akan diisi alamat node dari baris sekarang atau node yang sedang aktif(sedang ditempati cursor)
    // Dan untuk next nya dari node baru akan diisi alamat dari node next dari baris yang sedang aktif
    node_baru->prev = ed->baris_sekarang; 
    node_baru->next = ed->baris_sekarang->next; 

    // Kalo logika ini ketika next nya dari baris sekarang tidak NULL(ada node lagi) untuk nyambungnya jadi nanti alamat prev nya dari baris next (setelah) baris sekarang akan diisi si alamat node baru itu
    // Kalo NULL (kosong/cursor berada di ujung(tail)) alamat dari tail baris sekarang akan diisi alamat node baru
    if (ed->baris_sekarang->next != NULL) {
        ed->baris_sekarang->next->prev = node_baru; 
    } else {
        ed->tail = node_baru; 
    }
    
    // Logika ini murni untuk tampilan khusunya untuk cursor jadi ketika sudah buat node baru cursor yang tadinya ada di baris sekarang akan di pindahkan ke node baru di indeks ke 0 ujung kiri
    // Dan yang paling bawah untuk memberitahu program bahwa jumlah barisnya itu nambah 1
    ed->baris_sekarang = node_baru;
    ed->kolom_sekarang = 0;
    ed->jumlah_baris++;
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
            if (ed->kolom_sekarang >= MAX_KOLOM - 1) {
                tambahBarisBaru(ed); 
                posisi_y_layar++;    
                
                gotoxy(ed->kolom_sekarang, posisi_y_layar);
            }
            if (ed->kolom_sekarang < MAX_KOLOM - 1) {
                ed->baris_sekarang->info[ed->kolom_sekarang] = ch;
                printf("%c", ch); 
                
                ed->kolom_sekarang++;
                ed->baris_sekarang->jumlah_karakter++;
            }
        }
        else if (ch == 13){ // ENTER
			if (ed->jumlah_baris < MAX_BARIS){
				tambahBarisBaru(ed);
				posisi_y_layar++;
			
			}else{
			
			}
		} 
    }
}
