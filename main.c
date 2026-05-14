/*
 * Simulasi Antrian Pesanan SEMBAKO — Live TikTok Shop
 * Implementasi Arena Allocator + Struktur Data
 *
 * Kompilasi: gcc -o tiktok main.c arena.c order_array.c linked_list.c queue.c
 * Jalankan : ./tiktok
 *
 * Logika program:
 *   1. Siapkan blok memori statis = arena (ruang kerja 1 sesi live)
 *   2. Baca pesanan.csv → Array (database) + Linked List (urutan masuk)
 *   3. Queue → kumpulkan pesanan, proses per gelombang (batch flash sale)
 *   4. Skenario 1 → isi arena sampai penuh, lihat pesan overflow
 *   5. Skenario 2 → bandingkan reset arena O(1) vs malloc free satu-satu
 *   6. Analisis cache locality dicetak langsung ke output
 *
 * Hubungan dengan TikTok Shop:
 *   [NYATA]    TikTok API webhook → Backend → Arena → Proses
 *   [SIMULASI] pesanan.csv        → main.c  → Arena → Proses
 *   Arena = ruang kerja 1 sesi live. Sesi selesai = reset O(1).
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "arena.h"
#include "order_array.h"
#include "linked_list.h"
#include "queue.h"

#define ARENA_SIZE      2048
#define ARENA_EKS_SIZE  65536
#define MAX_CSV_PESANAN 50

// Baca pesanan.csv dan masukkan ke array + linked list
// Simulasi pesanan live TikTok yang datang real-time
static int baca_csv(const char *path, OrderArray *arr, LinkedList *list) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        printf("[PERINGATAN] %s tidak ditemukan, pakai data manual.\n\n", path);
        return 0;
    }

    char baris[128];
    int  total = 0;
    int  skip  = 1;  // lewati baris header

    while (fgets(baris, sizeof(baris), f) != NULL && total < MAX_CSV_PESANAN) {
        if (skip) { skip = 0; continue; }

        int  id, jumlah;
        char nama[50];

        if (sscanf(baris, "%d,%49[^,],%d", &id, nama, &jumlah) == 3) {
            insert_order(arr, id, nama, jumlah);
            ll_insert(list, id, nama, jumlah);
            total++;
        }
    }

    fclose(f);
    printf("[CSV] %d pesanan berhasil dibaca dari %s\n\n", total, path);
    return total;
}

int main(void) {

    uint8_t memory[ARENA_SIZE];
    Arena   arena;
    arena_init(&arena, memory, sizeof(memory));

    printf("==================================================\n");
    printf("  SISTEM ANTRIAN PESANAN SEMBAKO - LIVE TIKTOK\n");
    printf("  Implementasi Arena Allocator + Struktur Data\n");
    printf("==================================================\n");

    // ========================================================
    // PROGRESS 1 : ARENA ALLOCATOR
    // Tampilkan kondisi awal arena sebelum ada alokasi apapun
    // ========================================================
    printf("\n=== PROGRESS 1: ARENA ALLOCATOR ===\n");
    printf("[INFO] Arena siap. used=0, total=%zu byte\n",
       (size_t)ARENA_SIZE);
    arena_dump(&arena, "Kondisi Awal (Arena Kosong)");

    // ========================================================
    // PROGRESS 2 : ARRAY + LINKED LIST
    // Baca pesanan.csv → simpan ke array dan linked list
    // ========================================================
    printf("\n=== PROGRESS 2: ARRAY + LINKED LIST ===\n");
    printf("[INFO] Array       = database pembeli per sesi live\n");
    printf("[INFO] Linked List = urutan masuknya pesanan\n\n");

    OrderArray arr;
    if (!create_array(&arr, &arena, 20)) {
        printf("[ABORT] Gagal membuat array.\n");
        return 1;
    }

    LinkedList list;
    ll_init(&list, &arena);

    printf("\n[INFO] Membaca pesanan dari pesanan.csv...\n");
    int total_csv = baca_csv("pesanan.csv", &arr, &list);

    // fallback data manual jika CSV tidak ada
    if (total_csv == 0) {
        insert_order(&arr, 1, "Lahab 5kg", 2); ll_insert(&list, 1, "Lahab 5kg",       2);
        insert_order(&arr, 2, "Sunco 2L", 1); ll_insert(&list, 2, "Sunco 2L", 1);
        insert_order(&arr, 3, "KBA 1kg",  3); ll_insert(&list, 3, "KBA 1kg",  3);
        insert_order(&arr, 4, "Telur 1 Papan",   1); ll_insert(&list, 4, "Telur 1 Papan",   1);
        printf("\n");
    }

    print_orders(&arr);
    ll_print(&list);

    // contoh akses langsung via offset
    printf("[AKSES OFFSET LANGSUNG — Elemen ke-1]\n");
    size_t off_target = arr.base + 1 * sizeof(Order);
    Order *o = (Order *) arena_get(&arena, off_target);
    if (o)
        printf("  Offset %-4zu -> ID=%-3d | %-20s | x%d\n\n",
               off_target, o->id, o->nama, o->jumlah);

    arena_dump(&arena, "Setelah Array + Linked List");
    arena_status(&arena);

    // ========================================================
    // PROGRESS 3 : QUEUE + BATCH PROCESSING
    // Pesanan flash sale dikumpulkan di queue, diproses per batch
    // ========================================================
    printf("\n=== PROGRESS 3: QUEUE + BATCH PROCESSING ===\n");
    printf("[INFO] Reset arena untuk sesi flash sale...\n");
    arena_reset(&arena);
    arena_dump(&arena, "Sesi Live Berakhir, Antrian Dibersihkan");

    Queue q;
    queue_init(&q, &arena);

    printf("[INFO] Pesanan flash sale sembako masuk ke antrian...\n\n");
    enqueue(&q, 3001, "Lahab 5kg",        2);
    enqueue(&q, 3002, "Sunco 2L",  1);
    enqueue(&q, 3003, "KBA 1kg",   3);
    enqueue(&q, 3004, "Telur 1 Papan",    1);
    enqueue(&q, 3005, "Cakra 1kg", 2);

    queue_print(&q);
    arena_dump(&arena, "Setelah Queue Terisi");

    int batch_size = 3;
    int batch_ke   = 1;
    printf("[INFO] Mulai batch processing, batch_size=%d\n\n", batch_size);

    while (!queue_is_empty(&q)) {
        printf("==> Proses Gelombang Flash Sale ke-%d\n", batch_ke);
        int diproses = 0;
        while (diproses < batch_size && !queue_is_empty(&q)) {
            int  id, jml;
            char nama[50];
            dequeue(&q, &id, nama, &jml);
            diproses++;
        }
        printf("  Sisa antrian: %d pesanan\n\n", q.size);
        batch_ke++;
    }

    printf("[INFO] Semua pesanan selesai diproses.\n");
    arena_dump(&arena, "Setelah Semua Pesanan Diproses");
    arena_status(&arena);

    // ========================================================
    // PROGRESS 4 : SKENARIO UJI
    // ========================================================
    printf("\n=== PROGRESS 4: SKENARIO UJI ===\n");

    // SKENARIO 1 — Isi arena sampai penuh
    // Tujuan: lihat perilaku saat kapasitas habis (tidak crash)
    printf("\n[SKENARIO 1] Isi arena sampai batas penuh\n");
    arena_reset(&arena);
    queue_init(&q, &arena);
    arena_dump(&arena, "Reset Siap Skenario 1");

    printf("[INFO] Ukuran QueueNode = %zu byte\n", sizeof(QueueNode));
    printf("[INFO] Kapasitas arena  = %d byte\n", ARENA_SIZE);
    printf("[INFO] Maks pesanan     = %d / %zu = %zu pesanan\n\n",
           ARENA_SIZE, sizeof(QueueNode), (size_t)ARENA_SIZE / sizeof(QueueNode));

    int id_coba = 9001, berhasil = 0;
    while (1) {
        int hasil = enqueue(&q, id_coba, "Sembako Test", 1);
        if (hasil == -1) {
            printf("\n[SKENARIO 1 HASIL] Arena penuh pada ID=%d\n", id_coba);
            printf("[SKENARIO 1 HASIL] Total berhasil masuk: %d pesanan\n", berhasil);
            break;
        }
        berhasil++;
        id_coba++;
    }
    arena_dump(&arena, "Arena Penuh (Overflow Terdeteksi)");

    // SKENARIO 2 — Reset O(1) vs Malloc/Free
    // Tujuan: buktikan reset arena jauh lebih cepat dari free satu-satu
    printf("\n[SKENARIO 2] Perbandingan Reset Arena O(1) vs Malloc/Free\n");

    static uint8_t mem_eks[ARENA_EKS_SIZE];
    Arena arena_eks;
    arena_init(&arena_eks, mem_eks, sizeof(mem_eks));

    int N = 500;
    for (int i = 0; i < N; i++)
        arena_alloc(&arena_eks, sizeof(QueueNode));

    // ukur waktu reset arena
    clock_t t1 = clock();
    arena_reset(&arena_eks);
    clock_t t2 = clock();
    double ms_arena = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000.0;

    // ukur waktu malloc + free satu per satu
    QueueNode **ptrs = (QueueNode **) malloc(N * sizeof(QueueNode *));
    if (ptrs != NULL) {
        for (int i = 0; i < N; i++)
            ptrs[i] = (QueueNode *) malloc(sizeof(QueueNode));

        clock_t t3 = clock();
        for (int i = 0; i < N; i++) free(ptrs[i]);
        clock_t t4 = clock();
        free(ptrs);

        double ms_malloc = (double)(t4 - t3) / CLOCKS_PER_SEC * 1000.0;

        printf("\n  Jumlah node diuji    : %d\n", N);
        printf("  Arena reset          : %.6f ms  (O(1) — cuma set used=0)\n", ms_arena);
        printf("  Malloc/free per node : %.6f ms  (O(n) — free satu per satu)\n", ms_malloc);
        printf("  Catatan: perbedaan makin jelas dengan N yang jauh lebih besar.\n\n");
    }

    arena_dump(&arena_eks, "Setelah Reset O(1) (Skenario 2)");

    // ========================================================
    // PROGRESS 5 : ANALISIS CACHE LOCALITY
    // ========================================================
    printf("\n=== PROGRESS 5: ANALISIS CACHE LOCALITY ===\n\n");

    printf("  Array di Arena\n");
    printf("  Data berjejer RAPAT: [Order0][Order1][Order2][Order3]...\n");
    printf("  CPU baca 1 cache line = ambil beberapa elemen sekaligus\n");
    printf("  -> cache miss sedikit -> akses CEPAT\n\n");

    printf("  Linked List dengan malloc biasa\n");
    printf("  Tiap node di-malloc terpisah, posisi menyebar di memori:\n");
    printf("  [Node1: 0x1A00] -> [Node2: 0x3F80] -> [Node3: 0x2210]\n");
    printf("  CPU fetch tiap node dari lokasi beda\n");
    printf("  -> cache miss sering -> akses LAMBAT\n\n");

    printf("  Linked List di Arena (kode kita)\n");
    printf("  Node dialokasikan berurutan: [off=0][off=64][off=128]\n");
    printf("  Lebih baik dari malloc biasa, tapi tidak sebaik array.\n\n");

    printf("  Ranking: Array Arena > Linked List Arena > Linked List malloc\n");

    printf("\n==================================================\n");
    printf("  DEMO SELESAI\n");
    printf("  Semua struktur data berjalan di dalam arena.\n");
    printf("  Tidak ada malloc/free di level struktur data.\n");
    printf("==================================================\n");

    return 0;
}