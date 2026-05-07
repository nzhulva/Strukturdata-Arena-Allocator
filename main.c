#include <stdio.h>
#include <stdint.h>
#include "arena.h"
#include "order_array.h"
#include "linked_list.h"
#include "queue.h"

int main() {

    // 1. Siapkan arena (memori utama)
    uint8_t memory[1024];
    Arena arena;
    arena_init(&arena, memory, sizeof(memory));

    printf("\n=== PROGRESS 2: ARRAY DALAM ARENA ===\n");

    // 2. Tampilkan kondisi awal
    printf("\nKondisi awal:\n");
    arena_status(&arena);

    // 3. Buat array pesanan (kapasitas 5)
    OrderArray arr;
    if (!create_array(&arr, &arena, 5)) {
        printf("Gagal membuat array\n");
        return 1;
    }

    // 4. Tambahkan beberapa data pesanan
    printf("\nMenambahkan pesanan:\n");
    insert_order(&arr, 1001, "Nasi Goreng", 2);
    insert_order(&arr, 1002, "Es Teh", 3);
    insert_order(&arr, 1003, "Ayam Bakar", 1);

    // 5. Tampilkan isi array
    printf("\nIsi array:\n");
    print_orders(&arr);

    // 6. Contoh akses manual pakai offset (data ke-2)
    size_t off = arr.base + 1 * sizeof(Order);
    Order *o = (Order*) arena_get(&arena, off);

    if (o != NULL) {
        printf("Akses offset %zu -> %s (%d)\n",
               off, o->nama, o->jumlah);
    }

    // 7. Status setelah dipakai
    printf("\nStatus arena:\n");
    arena_status(&arena);

    printf("\n--- LINKED LIST ---\n");

    LinkedList list;
    ll_init(&list, &arena);

    ll_insert(&list, 2001, "Hoodie", 2);
    ll_insert(&list, 2002, "Kaos", 1);
    ll_insert(&list, 2003, "Topi", 4);

    ll_print(&list);

    // 9. Reset arena
    printf("\nReset arena...\n");
    arena_reset(&arena);

    // 10. Cek kondisi setelah reset
    printf("\nSetelah reset:\n");
    arena_status(&arena);

    // WEEK 3: Batch Processing menggunakan Queue dengan simulasi antrian pesanan (TikTok Live)

    printf("\n=== PROGRESS 3: QUEUE + BATCH PROCESSING (TIKTOK LIVE) ===\n");

    // Arena sudah di-reset, siap dipakai ulang dari awal
    Queue q;
    queue_init(&q, &arena);

    printf("\n--- Gelombang Pesanan Masuk (Live TikTok) ---\n");
    enqueue(&q, 3001, "Baju Batik",    2);
    enqueue(&q, 3002, "Sepatu Kets",   1);
    enqueue(&q, 3003, "Tas Selempang", 3);
    enqueue(&q, 3004, "Jaket Denim",   1);
    enqueue(&q, 3005, "Kaos Polos",    4);

    queue_print(&q);
    arena_status(&arena);

    // --- Batch processing: proses 3 pesanan per batch ---
    int batch_size = 3;
    int batch_ke   = 1;

    printf("--- Mulai Batch Processing (batch_size = %d) ---\n\n", batch_size);

    while (!queue_is_empty(&q)) {
        printf("[BATCH #%d]\n", batch_ke);

        int diproses = 0;
        while (diproses < batch_size && !queue_is_empty(&q)) {
            int id, jumlah;
            char nama[50];
            dequeue(&q, &id, nama, &jumlah);
            printf("  -> Diproses: ID=%-4d | %-20s | jumlah=%d\n",
                   id, nama, jumlah);
            diproses++;
        }

        printf("  Sisa antrian: %d pesanan\n\n", q.size);
        batch_ke++;
    }

    printf("Semua pesanan selesai diproses.\n");
    arena_status(&arena);

    // --- Reset dan sesi live baru ---
    printf("\n--- Reset Arena (sesi live baru) ---\n");
    arena_reset(&arena);
    queue_init(&q, &arena);

    enqueue(&q, 4001, "Hoodie Oversize", 1);
    enqueue(&q, 4002, "Celana Cargo",    2);

    printf("\nAntrian sesi baru:\n");
    queue_print(&q);
    arena_status(&arena);

    return 0;
}