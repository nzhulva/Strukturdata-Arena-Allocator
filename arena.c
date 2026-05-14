#include "arena.h"

// Hubungkan arena ke blok memori statis dari main
void arena_init(Arena *ar, uint8_t *mem, size_t size) {
    ar->memory = mem;
    ar->total  = size;
    ar->used   = 0;
}

// Reservasi sejumlah byte, kembalikan offset awal blok
// Kalau penuh kembalikan -1, program tidak crash
size_t arena_alloc(Arena *ar, size_t req) {
    if (ar->used + req > ar->total) {
        printf("[ERROR] Arena penuh! Pesanan tidak bisa masuk.\n");
        return (size_t)-1;
    }
    size_t pos = ar->used;
    ar->used  += req;
    return pos;
}

// Ubah offset jadi pointer nyata (memory + offset)
void *arena_get(Arena *ar, size_t offset) {
    if (offset >= ar->total) {
        printf("[ERROR] Offset %zu di luar arena\n", offset);
        return NULL;
    }
    return (void *)(ar->memory + offset);
}

// Reset arena ke nol — O(1), cukup set used=0
// Tidak ada loop, tidak ada free, sesederhana itu
void arena_reset(Arena *ar) {
    ar->used = 0;
    printf("[RESET] Arena bersih. used = 0 (O(1))\n");
}

// Tampilkan progress bar penggunaan memori
void arena_status(Arena *ar) {
    int persen    = (int)((ar->used * 100) / ar->total);
    int total_bar = 30;
    int filled    = (int)(ar->used * total_bar / ar->total);

    printf("\n--- Status Memori Arena ---\n");
    printf("Kapasitas : %zu byte\n", ar->total);
    printf("Terpakai  : %zu byte (%d%%)\n", ar->used, persen);
    printf("Sisa      : %zu byte\n", ar->total - ar->used);
    printf("[");
    for (int i = 0; i < total_bar; i++)
        printf(i < filled ? "#" : ".");
    printf("]\n");
    printf("# = terpakai | . = kosong\n\n");
}

// Visualisasi isi arena dalam grid 2D ASCII
// Format 16 kolom per baris: [000] ## ## .. ..
// ## = terpakai, .. = kosong
// Maks 8 baris ditampilkan supaya tidak banjir layar
void arena_dump(Arena *ar, const char *label) {
    const int COLS    = 16;
    size_t    max_row = (ar->total + COLS - 1) / COLS;
    if (max_row > 16) max_row = 16;

    printf("\n======================================\n");
    printf("ARENA DUMP : %s\n", label);
    printf("======================================\n");
    printf("Total: %zu | Terpakai: %zu | Kosong: %zu\n",
           ar->total, ar->used, ar->total - ar->used);
    printf("--------------------------------------\n");

    printf("Offset   ");
    for (int c = 0; c < COLS; c++) printf("%2X ", c);
    printf("\n");

    for (size_t row = 0; row < max_row; row++) {
        printf("[%03zu]    ", row * COLS);
        for (int col = 0; col < COLS; col++) {
            size_t idx = row * COLS + col;
            if (idx >= ar->total)    printf("   ");
            else if (idx < ar->used) printf("## ");
            else                     printf(".. ");
        }
        printf("\n");
    }

    if (ar->total > (size_t)(max_row * COLS))
        printf("... (%zu byte berikutnya tidak ditampilkan)\n",
               ar->total - max_row * COLS);

    printf("======================================\n");
    printf("## = terpakai | .. = kosong\n");
    printf("======================================\n\n");
}