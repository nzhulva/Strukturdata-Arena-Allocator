#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "arena.h"

typedef struct {
    int id;
    char nama[50];
    int jumlah;
    int next; // OFFSET
} OrderNode;

typedef struct {
    Arena *ar;
    int head;
} LinkedList;

void ll_init(LinkedList *list, Arena *ar);
int ll_insert(LinkedList *list, int id, const char *nama, int jumlah);
void ll_print(LinkedList *list);

#endif