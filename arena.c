#include "arena.h"

void arena_init(Arena *ar, uint8_t *mem, size_t size) {
    ar->memory = mem;
    ar->total  = size;
    ar->used   = 0;
}

size_t arena_alloc(Arena *ar, size_t req) {
    if (ar->used + req > ar->total) {
        printf("GAGAL: Gudang pesanan penuh, tidak bisa tampung pesanan baru.\n");
        return (size_t)-1;
    }
    size_t pos = ar->used;
    ar->used += req;
    printf("Slot pesanan dibuat: %zu byte di rak ke-%zu\n", req, pos);
    return pos;
}

void* arena_get(Arena *ar, size_t offset) {
    if (offset >= ar->total) {
        printf("ERROR: Rak ke-%zu di luar gudang\n", offset);
        return NULL;
    }
    return (void*)(ar->memory + offset);
}

void arena_reset(Arena *ar) {
    ar->used = 0;
    printf("Gudang pesanan dikosongkan (semua pesanan lama dihapus).\n");
}

void arena_status(Arena *ar) {
    printf("\n---------- STATUS GUDANG PESANAN ----------\n");
    printf("Kapasitas gudang : %zu byte\n", ar->total);
    printf("Sudah dipakai    : %zu byte\n", ar->used);
    printf("Sisa muat        : %zu byte\n", ar->total - ar->used);

    printf("Isi rak          : ");
    for (size_t i = 0; i < ar->total && i < 50; i++) {
        if (i < ar->used) printf("#");
        else              printf(".");
    }
    if (ar->total > 50) printf("...");
    printf("\n");
    printf("(# = ada pesanan, . = rak kosong)\n");
    printf("-------------------------------------------\n\n");
}