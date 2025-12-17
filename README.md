# FINAL-PROJECT-STRUKTUR-DATA-DAN-ANALISA-ALGORITMA

Proyek ini merupakan simulasi fisika 2D menggunakan C++ dan library SFML. Program ini bertujuan untuk mensimulasikan pergerakan banyak objek serta membandingkan kinerja dua algoritma collision detection yang berbeda.

#  Ball Collision Simulation — Bruteforce vs Quadtree (SFML)

Project ini merupakan simulasi **tabrakan bola 2D** menggunakan **C++ dan SFML**, dengan tujuan **membandingkan performa dan cara kerja metode Bruteforce dan Quadtree** dalam collision detection.

Simulasi ini menampilkan bola dengan **ukuran acak**, **kecepatan acak**, serta **gaya hambat (drag ringan)** agar bola dapat melambat dan berhenti secara natural tanpa gravitasi vertikal.

---

##  Tujuan Project

* Mempelajari perbedaan **collision detection Bruteforce dan Quadtree**
* Mengamati **perbedaan FPS** pada jumlah objek yang besar
* Menerapkan **optimasi spasial** menggunakan struktur data Quadtree
* Menyajikan simulasi visual interaktif menggunakan **SFML**

---

##  Fitur Utama

* ✅ Simulasi bola 2D dengan **radius acak**
* ✅ **Bruteforce Collision Detection**
* ✅ **Quadtree Collision Detection**
* ✅ Toggle mode dengan tombol **TAB**
* ✅ Tambah bola dengan **klik kiri mouse**
* ✅ **FPS dihitung per 1000 frame** (lebih stabil & mudah dibandingkan)
* ✅ Drag ringan agar bola tidak terus menambah kecepatan
* ✅ Warna bola acak
* ✅ Tampilan mode & FPS di layar

---

## Kontrol



Klik Kiri Mouse = Menambah bola (radius & kecepatan acak) 

TAB             = Ganti mode Bruteforce / Quadtree     \

Close Window    = Keluar program                          

---

##  Teknologi yang Digunakan

* **Bahasa**: C++
* **Library Grafik**: SFML
* **Compiler**: MinGW (g++)
* **Platform**: Windows

---

##  Perbandingan Metode

###  Bruteforce

* Mengecek **semua pasangan bola**
* Kompleksitas waktu: **O(n²)**
* Mudah diimplementasikan
* FPS turun drastis saat jumlah bola banyak

###  Quadtree

* Membagi area menjadi beberapa region
* Mengecek tabrakan hanya pada objek yang berdekatan
* Kompleksitas mendekati **O(n log n)**
* FPS jauh lebih stabil untuk objek besar

---



