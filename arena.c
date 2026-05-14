#include "arena.h"

void arena_init(Arena *ar, uint8_t *mem, size_t size) {
    ar->memory = mem;
    ar->total  = size;
    ar->used   = 0;
}

size_t arena_alloc(Arena *ar, size_t req) {
    if (ar->used + req > ar->total) {
        printf("[ERROR] Arena penuh! Tidak bisa tampung pesanan baru.\n");
        return (size_t)-1;
    }

    size_t pos = ar->used;
    ar->used += req;

    printf("[ALLOC] %zu byte di offset %zu\n", req, pos);
    return pos;
}

void* arena_get(Arena *ar, size_t offset) {
    if (offset >= ar->total) {
        printf("[ERROR] Offset %zu di luar arena\n", offset);
        return NULL;
    }

    return (void*)(ar->memory + offset);
}

void arena_reset(Arena *ar) {
    ar->used = 0;
    printf("[RESET] Arena dibersihkan. used = 0\n");
}

void arena_status(Arena *ar) {
    int persen = (int)((ar->used * 100) / ar->total);

    printf("\n==================================================\n");
    printf("               STATUS MEMORI ARENA\n");
    printf("==================================================\n");
    printf("Kapasitas : %zu byte\n", ar->total);
    printf("Terpakai  : %zu byte (%d%%)\n", ar->used, persen);
    printf("Sisa      : %zu byte\n", ar->total - ar->used);

    printf("\n[");
    int total_bar = 30;
    int filled = (int)(ar->used * total_bar / ar->total);

    for (int i = 0; i < total_bar; i++) {
        if (i < filled) printf("#");
        else printf(".");
    }

    printf("]\n");
    printf("# = terpakai | . = kosong\n");
    printf("==================================================\n\n");
}

void arena_dump(Arena *ar, const char *label) {
    const int COLS = 16;

    printf("\n==================================================\n");
    printf("ARENA DUMP : %s\n", label);
    printf("==================================================\n");
    printf("Total     : %zu byte\n", ar->total);
    printf("Terpakai  : %zu byte\n", ar->used);
    printf("Kosong    : %zu byte\n", ar->total - ar->used);
    printf("==================================================\n");

    printf("Offset   ");
    for (int c = 0; c < COLS; c++) {
        printf("%2X ", c);
    }
    printf("\n");

    size_t max_rows = (ar->total + COLS - 1) / COLS;
    if (max_rows > 8) max_rows = 8;

    for (size_t row = 0; row < max_rows; row++) {
        printf("0x%04zX : ", row * COLS);

        for (int col = 0; col < COLS; col++) {
            size_t idx = row * COLS + col;

            if (idx >= ar->total) {
                printf("   ");
            } else if (idx < ar->used) {
                printf(" # ");
            } else {
                printf(" . ");
            }
        }

        printf("\n");
    }

    if (ar->total > (size_t)(max_rows * COLS)) {
        printf("... (%zu byte berikutnya tidak ditampilkan)\n",
               ar->total - max_rows * COLS);
    }

    printf("==================================================\n");
    printf("# = terpakai | . = kosong\n");
    printf("==================================================\n\n");
}