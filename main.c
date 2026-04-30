#include <stdio.h>
#include <stdint.h>
#include "arena.h"

int main() {
    uint8_t memory[1024];
    Arena arena;

    // Inisialisasi arena
    arena_init(&arena, memory, sizeof(memory));

    printf("\n=== PROGRESS WEEK 1: ARENA ALLOCATOR ===\n");

    // Kondisi awal
    printf("\nKondisi awal arena:\n");
    arena_status(&arena);

    // Alokasi memori untuk 1 integer
    size_t offset = arena_alloc(&arena, sizeof(int));

    if (offset == (size_t)-1) {
        return 1;
    }

    // Mengambil alamat dari arena
    int *data = (int*) arena_get(&arena, offset);

    // Isi data sederhana
    *data = 1001;

    printf("\nData berhasil disimpan di arena\n");
    printf("Nilai data: %d\n", *data);

    // Setelah alokasi
    printf("\nSetelah alokasi:\n");
    arena_status(&arena);

    // Reset arena
    printf("\nReset arena...\n");
    arena_reset(&arena);

    // Setelah reset
    printf("\nSetelah reset:\n");
    arena_status(&arena);

    return 0;
}