#include <stdio.h>
#include <string.h>
#include "linked_list.h"

// Inisialisasi linked list kosong
// head = -1 artinya belum ada node sama sekali
void ll_init(LinkedList *list, Arena *ar) {
    list->ar   = ar;
    list->head = -1;
}

// Tambah node baru ke akhir linked list
// Node disimpan di arena, bukan malloc biasa
int ll_insert(LinkedList *list, int id, const char *nama, int jumlah) {

    // Minta slot memori baru dari arena
    size_t offset = arena_alloc(list->ar, sizeof(OrderNode));

    // Kalau arena penuh -> gagal tambah node
    if (offset == (size_t)-1) {
        printf("[LINKED LIST] Arena penuh.\n");
        return -1;
    }

    // Ambil alamat node dari offset arena
    OrderNode *node = (OrderNode *) arena_get(list->ar, offset);

    // Isi data node
    node->id     = id;
    node->jumlah = jumlah;
    node->next   = -1;

    // Copy nama dengan aman
    strncpy(node->nama, nama ? nama : "", 49);
    node->nama[49] = '\0';

    // Kalau list masih kosong
    // node baru langsung jadi head
    if (list->head == -1) {
        list->head = (int) offset;
        return (int) offset;
    }

    // Cari node terakhir
    int current = list->head;

    while (1) {

        OrderNode *temp =
            (OrderNode *) arena_get(list->ar, current);

        // Kalau next = -1 berarti node terakhir ditemukan
        if (temp->next == -1)
            break;

        current = temp->next;
    }

    // Sambungkan node terakhir ke node baru
    OrderNode *last =
        (OrderNode *) arena_get(list->ar, current);

    last->next = (int) offset;

    return (int) offset;
}

// Tampilkan seluruh isi linked list
// Traversal dimulai dari head sampai next = -1
void ll_print(LinkedList *list) {

    printf("\n=== LINKED LIST PESANAN ===\n");

    // Kalau kosong
    if (list->head == -1) {
        printf("(linked list kosong)\n");
        printf("===========================\n\n");
        return;
    }

    int current = list->head;

    while (current != -1) {

        // Ambil node dari arena berdasarkan offset
        OrderNode *node =
            (OrderNode *) arena_get(list->ar, current);

        // Tampilkan isi node
        printf("Offset=%-4d | ID=%-3d | %-20s | jumlah=%d\n",
               current,
               node->id,
               node->nama,
               node->jumlah);

        // Pindah ke node berikutnya
        current = node->next;
    }

    printf("NULL\n");
    printf("===========================\n\n");
}