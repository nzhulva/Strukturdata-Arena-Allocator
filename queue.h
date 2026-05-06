#ifndef QUEUE_H
#define QUEUE_H

#include "arena.h"

// Satu node di dalam antrian
typedef struct {
    int id;         // ID pesanan
    char nama[50];  // nama item
    int jumlah;     // jumlah item
    int next;       // offset node berikutnya (-1 = akhir)
} QueueNode;

// Struct antrian (FIFO)
typedef struct {
    Arena *ar;  // arena tempat data disimpan
    int front;  // offset node terdepan (keluar duluan)
    int rear;   // offset node terbelakang (masuk terakhir)
    int size;   // jumlah pesanan dalam antrian
} Queue;

// Inisialisasi antrian kosong
void queue_init(Queue *q, Arena *ar);

// Cek apakah antrian kosong
int queue_is_empty(Queue *q);

// Masukkan pesanan ke belakang antrian
int enqueue(Queue *q, int id, const char *nama, int jumlah);

// Keluarkan pesanan dari depan antrian (FIFO)
int dequeue(Queue *q, int *out_id, char *out_nama, int *out_jumlah);

// Tampilkan semua isi antrian dari depan ke belakang
void queue_print(Queue *q);

#endif