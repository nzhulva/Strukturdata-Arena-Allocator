#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "arena.h"

// Satu node dalam linked list
// next bukan pointer biasa — isinya OFFSET ke node berikutnya
// -1 berarti akhir list
typedef struct {
    int  id;
    char nama[50];
    int  jumlah;
    size_t  next;  // offset node berikutnya di arena
} OrderNode;

// Control struct (hidup di luar arena)
typedef struct {
    Arena *ar;
    size_t    head;  // offset node pertama, -1 = kosong
} LinkedList;

void ll_init   (LinkedList *list, Arena *ar);
int  ll_insert (LinkedList *list, int id, const char *nama, int jumlah);
void ll_print  (LinkedList *list);

#endif