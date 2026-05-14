#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "arena.h"
#include "order_array.h"
#include "linked_list.h"
#include "queue.h"

void tambah_pesanan(OrderArray *arr, LinkedList *list,
                    int id, const char *nama, int jumlah) {
    insert_order(arr, id, nama, jumlah);
    ll_insert(list, id, nama, jumlah);
}

int main() {

    uint8_t memory[2048];
    Arena arena;
    arena_init(&arena, memory, sizeof(memory));

    printf("==================================================\n");
    printf(" SISTEM ANTRIAN PESANAN LIVE TIKTOK SHOPPING\n");
    printf(" Implementasi Arena Allocator + Struktur Data\n");
    printf("==================================================\n");

    printf("\n=== PROGRESS 1: ARENA ALLOCATOR ===\n");
    printf("[INFO] Arena siap. used=0, total=%zu byte\n",
           sizeof(memory));

    arena_dump(&arena, "Kondisi Awal");

    printf("\n=== PROGRESS 2: ARRAY DAN LINKED LIST ===\n");
    printf("[INFO] Array = database pembeli\n");
    printf("[INFO] Linked List = urutan antrian\n\n");

    OrderArray arr;

    if (!create_array(&arr, &arena, 10)) {
        printf("Gagal membuat array\n");
        return 1;
    }

    LinkedList list;
    ll_init(&list, &arena);

    printf("[INFO] Pesanan masuk\n\n");

    tambah_pesanan(&arr, &list, 1, "Orang A", 2);
    tambah_pesanan(&arr, &list, 2, "Orang B", 1);
    tambah_pesanan(&arr, &list, 3, "Orang A", 3);
    tambah_pesanan(&arr, &list, 4, "Orang C", 1);

    printf("\n[ARRAY]\n");
    print_orders(&arr);

    printf("[LINKED LIST]\n");
    ll_print(&list);

    printf("\n[AKSES OFFSET]\n");

    size_t off = arr.base + 1 * sizeof(Order);
    Order *o = (Order*) arena_get(&arena, off);

    if (o != NULL) {
        printf("offset %zu -> ID=%d | %s | jumlah=%d\n",
               off, o->id, o->nama, o->jumlah);
    }

    arena_dump(&arena, "Setelah Array + Linked List");
    arena_status(&arena);

    arena_reset(&arena);
    arena_dump(&arena, "Setelah Reset");

    printf("\n=== PROGRESS 3: QUEUE + BATCH PROCESSING ===\n");

    Queue q;
    queue_init(&q, &arena);

    printf("[INFO] Arena dipakai ulang dari offset 0\n");

    printf("\n[INFO] Pesanan masuk antrian\n\n");

    enqueue(&q, 3001, "Orang A", 2);
    enqueue(&q, 3002, "Orang B", 1);
    enqueue(&q, 3003, "Orang A", 3);
    enqueue(&q, 3004, "Orang C", 1);
    enqueue(&q, 3005, "Orang D", 4);

    queue_print(&q);

    arena_dump(&arena, "Setelah Queue");

    int batch_size = 3;
    int batch_ke = 1;

    printf("[INFO] Flash Sale diproses batch_size=%d\n\n",
           batch_size);

    while (!queue_is_empty(&q)) {

        printf("Batch #%d\n", batch_ke);

        int diproses = 0;

        while (diproses < batch_size &&
               !queue_is_empty(&q)) {

            int id, jumlah;
            char nama[50];

            dequeue(&q, &id, nama, &jumlah);

            printf("Diproses -> ID=%d | %s | jumlah=%d\n",
                   id, nama, jumlah);

            diproses++;
        }

        printf("Sisa antrian: %d\n\n", q.size);

        batch_ke++;
    }

    printf("[INFO] Semua pesanan selesai diproses\n");

    arena_dump(&arena, "Setelah Semua Diproses");
    arena_status(&arena);

    arena_reset(&arena);
    queue_init(&q, &arena);

    arena_dump(&arena, "Reset Siap Sesi Baru");

    printf("\n=== PROGRESS 4: EKSPERIMEN ===\n");

    printf("\n[EKSPERIMEN 1] Isi arena sampai penuh\n");

    printf("[INFO] QueueNode = %zu byte\n",
           sizeof(QueueNode));

    printf("[INFO] Sisa arena = %zu byte\n",
           arena.total - arena.used);

    int id_coba = 9001;
    int berhasil = 0;

    while (1) {

        int hasil = enqueue(&q, id_coba, "Test", 1);

        if (hasil == -1) {
            printf("[INFO] Arena penuh pada ID=%d\n",
                   id_coba);
            break;
        }

        berhasil++;
        id_coba++;
    }

    printf("[HASIL] Berhasil masuk: %d\n", berhasil);

    arena_dump(&arena, "Arena Penuh");

    printf("\n[EKSPERIMEN 2] Reset arena O(1)\n");

    arena_reset(&arena);
    queue_init(&q, &arena);

    arena_dump(&arena, "Setelah Reset O(1)");

    printf("\n[EKSPERIMEN 3] Sesi live baru\n");

    enqueue(&q, 4001, "Orang A", 1);
    enqueue(&q, 4002, "Orang B", 2);
    enqueue(&q, 4003, "Orang C", 1);

    queue_print(&q);

    arena_dump(&arena, "Sesi Live Baru");

    arena_status(&arena);

    printf("\n==================================================\n");
    printf(" DEMO SELESAI\n");
    printf("==================================================\n");

    return 0;
}