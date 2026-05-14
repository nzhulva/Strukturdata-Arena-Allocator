#ifndef QUEUE_H
#define QUEUE_H

#include "arena.h"

// Satu node dalam antrian — next pakai offset bukan pointer
typedef struct {
    int  id;
    char nama[50];
    int  jumlah;
    int  next;  // offset node berikutnya, -1 = akhir
} QueueNode;

// Control struct antrian FIFO
typedef struct {
    Arena *ar;
    int    front;  // offset node terdepan (keluar duluan)
    int    rear;   // offset node terbelakang (masuk terakhir)
    int    size;
} Queue;

void queue_init     (Queue *q, Arena *ar);
int  queue_is_empty (Queue *q);
int  enqueue        (Queue *q, int id, const char *nama, int jumlah);
int  dequeue        (Queue *q, int *out_id, char *out_nama, int *out_jumlah);
void queue_print    (Queue *q);

#endif