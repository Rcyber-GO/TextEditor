#include "alpinheader.h"
#include <windows.h> 
#include <conio.h>

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Fungsi inisialisasi yang udah ada sebelumnya dan hanya dimodifikasi sedikit untuk menyesuaikan dengan kebutuhan
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

void tanganiRipple(TextEditor *ed, int posisi_y_layar) {
	// Disini kita membuat sebuah pointer penanda bernama temp yg posisi awalnya itu akan ditaruh dibaris yang sedang ditempati cursor
    Node *temp = ed->baris_sekarang;
    
    // Nah disini program bakal ngecek selama si jumlah karakter dibaris yang sudah ditandai temp ini lebih dari max kolom
    // Program akan mengeksekusi kode didalam logika while dimana karakter +1 dari max kolom atau bisa disebut karakter yg melebihi jumlah array akan disimpan di variabel bernama tumpahan
    // Lalu setelah karakter tadi dipindah ke variabel tumpahan, baris tadi akan mengganti karakter yg diculik tadi dengan \0 supaya max kolomnya kembali pas dan tak berlebih
    while (temp->jumlah_karakter > MAX_KOLOM - 1) {
        char tumpahan = temp->info[MAX_KOLOM - 1]; 
        temp->info[MAX_KOLOM - 1] = '\0';
        temp->jumlah_karakter--;

	// JIka dibawah baris yang ditandai temp ini tidak ada baris lagi maka dia akan membuat node baru dan akan memesan memori untuk menaruh node baru tersebut
        if (temp->next == NULL) {
            Node *node_baru = (Node*)malloc(sizeof(Node));
            memset(node_baru->info, 0, MAX_KOLOM);
            node_baru->jumlah_karakter = 0;

	// Nah disini node baru tadi pastinya harus disambungkan sama node temp tadi makanya ada logika prev dari node barunya diisi sama node temp dan nextnya null karna belum ada baris lagi dibawahnya
	// Otomatis tailnya berubah dari yang tadinya di node temp berubah ke node baru yang berada paling ujung kanan
            node_baru->prev = temp;
            node_baru->next = NULL;
            temp->next = node_baru;
            ed->tail = node_baru;
            ed->jumlah_baris++;
        }
	
	// Nah kalo misal ternyata ada baris lagi dibawahnya pointer tempnya akan menunjuk ke baris selanjutnya yg ada dibawah 
	// Sebelum si karakter yg disimpan ditumpahan dipindahkan ke baris ke 2. disini saya harus menggeser seluruh karakter dibaris bawah bawahnya supaya menciptakan 1 ruang ksosong bagi karakter tumpahan
	// Jika sudah kosong dan ada ruang maka karakter tumpahannya akan disimpan di indeks ke 0 pada baris ke 2
        temp = temp->next;
        memmove(&temp->info[1], &temp->info[0], temp->jumlah_karakter);
        temp->info[0] = tumpahan;
        temp->jumlah_karakter++;
    }

    // Kalo ini logika buat visualisasi di CLI nya. kenapa saya pakai karna kalo gak pakai ini ketika saya jalankan programnya di CLI ketika karakter bergeser di CLi malah cuman diem doang tapi sebenenarnya kalo kita buka dinotepad dia bergeser aslinya
    //
    gotoxy(ed->kolom_sekarang - 1, posisi_y_layar);
    printf("%s ", &ed->baris_sekarang->info[ed->kolom_sekarang - 1]);

    temp = ed->baris_sekarang->next;
    int render_y = posisi_y_layar + 1;
    
    while(temp != NULL) {
        gotoxy(0, render_y);
        printf("%s ", temp->info); 
        temp = temp->next;
        render_y++;
    }

    gotoxy(ed->kolom_sekarang, posisi_y_layar);
}

void tanganiRippleMundur(TextEditor *ed, int posisi_y_layar) {
    Node *temp = ed->baris_sekarang;
    
    // Selama baris dibawah dari node temp gak kosong dia akan menjalankan sebuah perintah yang dimana dia membuat sebuah pointer baru bernama bawah yang tugasnya untuk menandai baris yang berada dibawah si node temp
    while (temp->next != NULL) {
        Node *bawah = temp->next;
    // Jika baris dibawah itu ada isinya indeks nya lebih dari 0 dia akan mengambil karakter pertama dari baris sibawah dan akan menaruhnya diujung kanan baris si node temp   
    // Lalu dia akan menambahkan jumlah karakter di baris node temp setelah berhasil mencuri atau mendapatkan karakter dari baris dibawahnya
        if (bawah->jumlah_karakter > 0) {
            temp->info[temp->jumlah_karakter] = bawah->info[0];
            temp->jumlah_karakter++;
            temp->info[temp->jumlah_karakter] = '\0';
            
    // Nah karna karakter pertama dari baris yg kedua tadi sudah dicuri atau pindah maka sisa huruf dibaris ke 2 dan baris baris dibawahnya akan digeser kekiri mengisi kekosongan dari indeks pertama baris ke 2       
            memmove(&bawah->info[0], &bawah->info[1], bawah->jumlah_karakter);
            bawah->jumlah_karakter--;
            bawah->info[bawah->jumlah_karakter] = '\0';
        }
        
    // Kalo ternyata tidak ada baris dibawahnya maka ketika karakternya pindah kebaris diatasnya maka baris yg dibawah akan dihapus
    // Dan akan memutuskan tali dari baris pertama ke baris ke 2
		if (bawah->jumlah_karakter == 0) {
            temp->next = bawah->next;
            if (bawah->next != NULL) {
                bawah->next->prev = temp;
                
            // Memindahkan tail nya ke baris pertama 
            } else {
                ed->tail = temp; 
            }
            
            // Mengosongkan memori karna baris dibawahnya sudah kosong dihapus
            free(bawah);
            ed->jumlah_baris--;
        }
        temp = temp->next;
        if (temp == NULL) break;
    }

    // RENDER VISUAL KE CLI
    gotoxy(ed->kolom_sekarang, posisi_y_layar);
    printf("%s  ", &ed->baris_sekarang->info[ed->kolom_sekarang]); 

    Node *render_node = ed->baris_sekarang->next;
    int render_y = posisi_y_layar + 1;
    while (render_node != NULL) {
        gotoxy(0, render_y);
        printf("%s  ", render_node->info);
        render_node = render_node->next;
        render_y++;
    }
    
    gotoxy(0, render_y);
    for(int i = 0; i < MAX_KOLOM; i++) printf(" "); 
    gotoxy(ed->kolom_sekarang, posisi_y_layar);
}
	
	// Ini tuh logika buat ketika backspace dan udah mentok kiri maka cursor akan naik keatas
void tanganiBackspaceKiri(TextEditor *ed, int *posisi_y_layar) {
	// Disini mmebuat 2 pointer baru yang dimana pointer sekarang akan menunjuk alamat dari baris yang sedang aktif (ditempati kursor) dan pointer atas akan menunjuk ke alamat dari baris yg ada di atasnya
    Node *atas = ed->baris_sekarang->prev;
    Node *sekarang = ed->baris_sekarang;
    
	// Logika untuk menempatkan cursor tepat di ujung kanan baris diatasnya
	// Disini juga dia menghitung berapa banyak slot kosong yang tersisa dibaris yg atasnya kalo misal masih banyak maka ketika baris yg dibawah cukup dipindahkan ke atas semua maka dia langsung memindahkannya semua bukan 1 per 1
    int target_x = atas->jumlah_karakter;
    int sisa_ruang = (MAX_KOLOM - 1) - atas->jumlah_karakter;
    int bisa_pindah = sekarang->jumlah_karakter;
    if (bisa_pindah > sisa_ruang) bisa_pindah = sisa_ruang;

    // Program akan ngecek apakah ada huruf yang bisa dipindahkan kalo misal baris atasnya sudah full banget maka kode bawahnya gak akan dijalankan
    if (bisa_pindah > 0) {
    	
        memmove(&atas->info[atas->jumlah_karakter], sekarang->info, bisa_pindah);
        atas->jumlah_karakter += bisa_pindah;
        atas->info[atas->jumlah_karakter] = '\0';

        memmove(&sekarang->info[0], &sekarang->info[bisa_pindah], sekarang->jumlah_karakter - bisa_pindah);
        sekarang->jumlah_karakter -= bisa_pindah;
        sekarang->info[sekarang->jumlah_karakter] = '\0';
    }

    // Menaikan cursor ke baris yg adaa diatas
    ed->baris_sekarang = atas;
    ed->kolom_sekarang = target_x;
    (*posisi_y_layar)--; 

    // Hapus baris kalau udah kosong dan memutuskan tali antar node
    if (sekarang->jumlah_karakter == 0) {
        atas->next = sekarang->next;
        if (sekarang->next != NULL) {
            sekarang->next->prev = atas;
        } else {
            ed->tail = atas;
        }
        free(sekarang);
        ed->jumlah_baris--;
    }

    // Render ulang dari baris yang dinaikin sampai ke bawah
    int render_y = *posisi_y_layar;
    Node *temp_render = ed->baris_sekarang;
    
    while (temp_render != NULL) {
        gotoxy(0, render_y);
        printf("%s", temp_render->info);
        for (int i = temp_render->jumlah_karakter; i < MAX_KOLOM; i++) printf(" ");
        temp_render = temp_render->next;
        render_y++;
    }
    
    gotoxy(0, render_y);
    for (int i = 0; i < MAX_KOLOM; i++) printf(" ");

    gotoxy(ed->kolom_sekarang, *posisi_y_layar);
}

void splitBaris(TextEditor *ed, int *posisi_y_layar) {
    // Disini program mengalokasikan satu slot memory untuk menampung kata yg terpotong 
    Node *newNode = (Node*)malloc(sizeof(Node));
    memset(newNode->info, 0, MAX_KOLOM);

    // Proses memotong datanya jadi dia akan memotong data sesuai cursor ditempatkan di sebelah man nahh data yg ada di sebelah kiri kursor akan tetap dibiarkan dibaris tersebut namun yg berada disebelah kanan akan diturunkan ke baris dibawahnya
    int sisa_panjang = ed->baris_sekarang->jumlah_karakter - ed->kolom_sekarang;
    
    if (sisa_panjang > 0) {
        memcpy(newNode->info, &ed->baris_sekarang->info[ed->kolom_sekarang], sisa_panjang);
        newNode->jumlah_karakter = sisa_panjang;
    } else {
        newNode->jumlah_karakter = 0;
    }

    // Bersihin baris lama di memori
    ed->baris_sekarang->info[ed->kolom_sekarang] = '\0';
    ed->baris_sekarang->jumlah_karakter = ed->kolom_sekarang;

    // Menayambungkan node baru ke baris yang sedang aktif
    newNode->prev = ed->baris_sekarang;
    newNode->next = ed->baris_sekarang->next;
    
    if (ed->baris_sekarang->next != NULL) {
        ed->baris_sekarang->next->prev = newNode;
    } else {
        ed->tail = newNode;
    }
    ed->baris_sekarang->next = newNode;
    
    // Hapus sisa teks di baris lama (yang udah dipotong ke baris baru)
    gotoxy(ed->kolom_sekarang, *posisi_y_layar);
    for(int i = 0; i < sisa_panjang; i++) printf(" ");

    // Pindah ke baris bawahnya
    (*posisi_y_layar)++;
    gotoxy(0, *posisi_y_layar);

    // Print isi baris baru
    printf("%s", newNode->info);
    
    // Bersihin sisa baris baru 
    for(int i = newNode->jumlah_karakter; i < MAX_KOLOM; i++) printf(" ");

    // Geser semua baris di bawah baris baru ke bawah 1 langkah 
    // karena ada baris baru yang muncul di tengah
    Node *temp = newNode->next;
    int y = *posisi_y_layar + 1;
    while(temp != NULL) {
        gotoxy(0, y);
        printf("%s", temp->info);
        for(int i = temp->jumlah_karakter; i < MAX_KOLOM; i++) printf(" ");
        temp = temp->next;
        y++;
    }

    ed->baris_sekarang = newNode;
    ed->kolom_sekarang = 0;
    ed->jumlah_baris++;

    // Balikin kursor ke posisi baris baru (0, y baru)
    gotoxy(ed->kolom_sekarang, *posisi_y_layar);
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
    
    // Logika untuk menyambungkan si node barunya itu ke nextnya dari baris sekarang yg sedang ditempati cursor jika tidak ada fungsi ini maka gerbong nodenya akan putus
    ed->baris_sekarang->next = node_baru;
    
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
		        // Hapus karakter di tengah
		        ed->kolom_sekarang--;
		        memmove(&ed->baris_sekarang->info[ed->kolom_sekarang], 
		                &ed->baris_sekarang->info[ed->kolom_sekarang + 1], 
		                ed->baris_sekarang->jumlah_karakter - ed->kolom_sekarang);
		        ed->baris_sekarang->jumlah_karakter--;
		        ed->baris_sekarang->info[ed->baris_sekarang->jumlah_karakter] = '\0';
		
		        tanganiRippleMundur(ed, posisi_y_layar);
    		} 
		    else if (ed->kolom_sekarang == 0 && ed->baris_sekarang->prev != NULL) {
		        // Gabung baris kalau mentok kiri
		        tanganiBackspaceKiri(ed, &posisi_y_layar); 
		    }
		}
        
        else if (ch == 224) { 
		    int key = _getch(); // Ambil kode tombol setelah 224
		    
		    // PANAH ATAS (72)
		    if (key == 72) {
		        if (ed->baris_sekarang->prev != NULL) {
		            ed->baris_sekarang = ed->baris_sekarang->prev;
		            posisi_y_layar--; // Geser kursor layar
		            if (ed->kolom_sekarang > ed->baris_sekarang->jumlah_karakter) {
		                ed->kolom_sekarang = ed->baris_sekarang->jumlah_karakter;
		            }
		        }
		    }
		    // PANAH BAWAH (80)
		    else if (key == 80) {
		        if (ed->baris_sekarang->next != NULL) {
		            ed->baris_sekarang = ed->baris_sekarang->next;
		            posisi_y_layar++; // Geser kursor layar
		            if (ed->kolom_sekarang > ed->baris_sekarang->jumlah_karakter) {
		                ed->kolom_sekarang = ed->baris_sekarang->jumlah_karakter;
		            }
		        }
		    }
		    // PANAH KIRI (75)
		    else if (key == 75) {
		        if (ed->kolom_sekarang > 0) {
		            ed->kolom_sekarang--;
		        } else if (ed->baris_sekarang->prev != NULL) {
		            ed->baris_sekarang = ed->baris_sekarang->prev;
		            ed->kolom_sekarang = ed->baris_sekarang->jumlah_karakter;
		            posisi_y_layar--;
		        }
		    }
		    // PANAH KANAN (77)
		    else if (key == 77) {
		        if (ed->kolom_sekarang < ed->baris_sekarang->jumlah_karakter) {
		            ed->kolom_sekarang++;
		        } else if (ed->baris_sekarang->next != NULL) {
		            ed->baris_sekarang = ed->baris_sekarang->next;
		            ed->kolom_sekarang = 0;
		            posisi_y_layar++;
		        }
		    }
		}
		
	   else if (ch >= 32 && ch <= 126) { // NGETIK BIASA
	    	if (ed->kolom_sekarang >= MAX_KOLOM - 1) {
	        if (ed->baris_sekarang->next == NULL) tambahBarisBaru(ed);
	        else {
	            ed->baris_sekarang = ed->baris_sekarang->next;
	            ed->kolom_sekarang = 0;
	        }
	        posisi_y_layar++;
	        gotoxy(ed->kolom_sekarang, posisi_y_layar);
			}
	
		    memmove(&ed->baris_sekarang->info[ed->kolom_sekarang + 1], 
		            &ed->baris_sekarang->info[ed->kolom_sekarang], 
		            ed->baris_sekarang->jumlah_karakter - ed->kolom_sekarang);
		    ed->baris_sekarang->info[ed->kolom_sekarang] = ch;
		    ed->baris_sekarang->jumlah_karakter++;
		    ed->kolom_sekarang++; 
		
		    tanganiRipple(ed, posisi_y_layar); 
		}
        else if (ch == 13){ // ENTER
			if (ed->jumlah_baris < MAX_BARIS){
			splitBaris(ed, &posisi_y_layar);	
			
			}
		} 
    }
}