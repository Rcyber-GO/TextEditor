### Projek 2: Mini Text Editor (C / CLI)
Program ini adalah teks editor sederhana berbasis Command Line Interface (CLI) yang dibuat menggunakan bahasa C. Projek ini cocok banget buat kita yang lagi belajar materi Struktur Data di semester 2, terutama implementasi Doubly Linked List dan operasi File Handling (baca/tulis file)
Fitur Program

### Program editor teks ini punya beberapa fitur utama yang fungsional:
- Buka File Eksisting: Program menanyakan apakah kita ingin membuka file di awal. Jika ya, program akan membaca file tersebut (misalnya .txt) baris per baris dan memuatnya ke dalam editor.
- Buat File Baru: Jika tidak ada file yang ingin dibuka, program akan membuatkan dokumen kosong yang baru.
- Mengetik & Menghapus: Kita bisa mengetik karakter biasa secara langsung di layar terminal dan menghapusnya menggunakan tombol Backspace.
- Batas Ketikan: Maksimal kapasitas dalam satu baris adalah 120 karakter. Jika lebih, kursor akan otomatis membuat baris baru di bawahnya.
- Batas Baris: Program mendukung penulisan hingga 1000 baris teks.
- Simpan (Save) File: Ketika kita selesai dan menekan tombol ESC, program akan meminta nama file tujuan untuk menyimpan hasil ketikan kita secara permanen.

### Konsep Struktur 
Program ini menggunakan Doubly Linked List sebagai pondasi utama untuk menyimpan teks.
- Setiap baris teks yang kita ketik direpresentasikan sebagai satu buah Node. 
- Setiap Node menyimpan array karakter (maksimal 120 karakter) dan informasi jumlah karakter di baris tersebut.
- Karena ini Doubly Linked List, setiap Node memiliki pointer next untuk menunjuk ke baris di bawahnya, dan pointer prev untuk menunjuk ke baris di atasnya.
- Logika Linked List ini sangat berguna saat kita harus menyisipkan atau menambah baris teks baru di tengah-tengah dokumen.

### Penjelasan File Source Code
Projek ini dipecah menjadi beberapa file agar rapi dan modular (enggak numpuk di satu tempat):
- alpinheader.h: File ini berisi definisi struct Node dan struct TextEditor. Di dalamnya juga terdapat deklarasi konstanta seperti MAX_BARIS (1000) dan MAX_KOLOM (120). Tempat dideklarasikannya fungsi-fungsi utama editor seperti inisialisasiEditor, jalankanEditor, dan tambahBarisBaru.
- Source Implementasi Editor Utama (berisi implementasi dari alpinheader.h) File ini menggunakan windows.h dan conio.h untuk mengatur posisi kursor di layar terminal dengan fungsi gotoxy. Berisi implementasi tambahBarisBaru yang menggunakan memori dinamis (malloc) untuk membuat node baru dan merangkainya ke dalam Linked List. Berisi logika utama jalankanEditor yang merupakan looping untuk menangkap input keyboard dari user (typing, Enter, Backspace, dan ESC).
- ikiheader.hFile header ini khusus untuk mendeklarasikan fungsi-fungsi terkait penyimpanan dan menu. Terdapat fungsi simpanFile, bukaFile, dan menuAwal.
- Source Fungsi File Handling (berisi implementasi dari ikiheader.h) Terdapat kode untuk membaca file (read) secara per baris menggunakan fungsi bukaFile, lalu mengubah setiap baris tersebut menjadi Node Linked List baru. Terdapat fungsi simpanFile yang bekerja dengan menelusuri isi Linked List dari node head sampai tail, lalu menuliskan isinya ke dalam file secara berurutan. Source Menu Antarmuka (Menu Awal)Fungsi menuAwal menanyakan pengguna "Apakah ingin membuka file? (Y/N)".  Memanggil fungsi inisialisasi awal atau fungsi membaca dokumen berdasarkan input pengguna.
- main.c Merupakan fungsi utama (main) yang pertama kali dijalankan.  Fungsi ini memanggil menuAwal untuk mulai, lalu jalankanEditor untuk proses ngetik.  Setelah keluar dari mode ngetik, program akan membersihkan layar, meminta nama file, dan memanggil simpanFile untuk menyimpan data sebelum program benar-benar berhenti.

### Cara Menjalankan Program
- Pastikan semua file ekstensi .c di-compile bersamaan (karena saling membutuhkan).
- Jalankan file .exe atau program utamanya.Ikuti instruksi di layar, mau buka file atau bikin baru.
- Ketik apapun yang kalian mau di dalam area editor.
- Tekan ESC kalau sudah beres.
- Ketik nama file untuk disave (jangan lupa tambahin ekstensinya, misalnya tugas.txt), lalu tekan Enter.
- Program akan memberikan notifikasi jika file berhasil disimpan. 
