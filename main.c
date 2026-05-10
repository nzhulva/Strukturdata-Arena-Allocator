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
 
    // INISIALISASI ARENA
 
    uint8_t memory[2048];
    Arena arena;
    arena_init(&arena, memory, sizeof(memory));
 
    printf("=======================================================\n");
    printf("   SISTEM ANTRIAN PESANAN LIVE TIKTOK                  \n");
    printf("   Implementasi Arena Allocator + Struktur Data         \n");
    printf("=======================================================\n");
 
    // PROGRESS 1: ARENA ALLOCATOR
 
    printf("\n=== PROGRESS 1: ARENA ALLOCATOR ===\n");
    printf("[INFO] Arena siap. used=0, total=%zu, semua slot kosong.\n",
           sizeof(memory));
    arena_status(&arena);
 
    // PROGReSS 2: ARRAY + LINKED LIST DALAM ARENA (new update: *connect array & LLnya (tambah_pesanan))
 
    printf("\n=== PROGRESS 2: ARRAY DAN LINKED LIST DALAM ARENA ===\n");
    printf("[INFO] Array       = database pembeli (siapa yang pesan)\n");
    printf("[INFO] Linked List = nomor urut antrian (urutan masuk)\n\n");
 
    OrderArray arr;
    if (!create_array(&arr, &arena, 10)) {
        printf("Gagal membuat array\n");
        return 1;
    }
 
    LinkedList list;
    ll_init(&list, &arena);
 
    printf("[PESANAN MASUK] Pembeli mulai memesan...\n\n");
 
    tambah_pesanan(&arr, &list, 1, "Orang A", 2);
    tambah_pesanan(&arr, &list, 2, "Orang B", 1);
    tambah_pesanan(&arr, &list, 3, "Orang A", 3);
    tambah_pesanan(&arr, &list, 4, "Orang C", 1);
 
    printf("\n[ARRAY] Database pembeli (siapa yang pesan):\n");
    print_orders(&arr);
 
    printf("[LINKED LIST] Nomor urut antrian (urutan masuk):\n");
    ll_print(&list);
 
    printf("\n[ARENA] Akses manual offset — baca data ke-2 dari array:\n");
    size_t off = arr.base + 1 * sizeof(Order);
    Order *o = (Order*) arena_get(&arena, off);
    if (o != NULL) {
        printf("  offset %zu -> ID=%d | %s | jumlah=%d\n",
               off, o->id, o->nama, o->jumlah);
    }
 
    printf("\n[ARENA] Status setelah Progress 2:\n");
    arena_status(&arena);
 
    printf("[RESET] Progress 2 selesai. Arena di-reset untuk Progress 3.\n");
    arena_reset(&arena);
    arena_status(&arena);
 
    // PROGRESS 3: QUEUE + BATCH PROCESSING (TIKTOK LIVE)
 
    printf("\n=== PROGRESS 3: QUEUE + BATCH PROCESSING (TIKTOK LIVE) ===\n");
 
    Queue q;
    queue_init(&q, &arena);
    printf("[QUEUE] Antrian aktif siap. Arena dipakai ulang dari offset 0.\n");
 
    printf("\n[LIVE] Gelombang pesanan masuk dari penonton TikTok:\n");
    enqueue(&q, 3001, "Orang A", 2);
    enqueue(&q, 3002, "Orang B", 1);
    enqueue(&q, 3003, "Orang A", 3);
    enqueue(&q, 3004, "Orang C", 1);
    enqueue(&q, 3005, "Orang D", 4);
 
    queue_print(&q);
 
    printf("[ARENA] Status setelah pesanan masuk antrian:\n");
    arena_status(&arena);
 
    int batch_size = 3;
    int batch_ke   = 1;
 
    printf("[BATCH] Mulai memproses antrian (batch_size=%d)...\n\n",
           batch_size);
 
    while (!queue_is_empty(&q)) {
        printf("[BATCH #%d]\n", batch_ke);
 
        int diproses = 0;
        while (diproses < batch_size && !queue_is_empty(&q)) {
            int id, jumlah;
            char nama[50];
            dequeue(&q, &id, nama, &jumlah);
            printf("  -> Diproses: ID=%-4d | %-10s | jumlah=%d\n",
                   id, nama, jumlah);
            diproses++;
        }
 
        printf("  Sisa antrian: %d pesanan\n\n", q.size);
        batch_ke++;
    }
 
    printf("[BATCH] Semua pesanan selesai diproses.\n");
    printf("\n[ARENA] Status setelah semua pesanan diproses:\n");
    arena_status(&arena);