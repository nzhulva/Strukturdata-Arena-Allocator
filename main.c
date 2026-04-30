#include <stdio.h>
#include <stdint.h>
#include "arena.h"
#include "order_array.h"

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
        printf("\nAkses offset %zu -> %s (%d)\n",
               off, o->nama, o->jumlah);
    }

    // 7. Status setelah dipakai
    printf("\nStatus arena:\n");
    arena_status(&arena);

    // 8. Dump visual (grid memori)
    printf("\nVisualisasi arena:\n");
    arena_dump(&arena);

    // 9. Reset arena
    printf("\nReset arena...\n");
    arena_reset(&arena);

    // 10. Cek kondisi setelah reset
    printf("\nSetelah reset:\n");
    arena_status(&arena);

    return 0;
}