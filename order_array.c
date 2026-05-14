#include <stdio.h>
#include <string.h>
#include "order_array.h"

// Buat array di arena — alokasi kapasitas*sizeof(Order) sekaligus
// Semua elemen nanti berjejer rapat (kontigu) = bagus untuk cache
int create_array(OrderArray *a, Arena *ar, size_t kapasitas) {
    if (a == NULL || ar == NULL || kapasitas == 0) return 0;

    size_t pos = arena_alloc(ar, kapasitas * sizeof(Order));
    if (pos == (size_t)-1) return 0;

    a->ar        = ar;
    a->base      = pos;
    a->kapasitas = kapasitas;
    a->jumlah    = 0;

    printf("[ARRAY] Dibuat di offset %zu (maks %zu pesanan)\n", pos, kapasitas);
    return 1;
}

// Tambah pesanan ke slot berikutnya
// Offset dihitung: base + jumlah * sizeof(Order)
size_t insert_order(OrderArray *a, int id, const char *nama, int jumlah) {
    if (a == NULL || a->ar == NULL) return (size_t)-1;
    if (a->jumlah >= a->kapasitas) {
        printf("[ARRAY] Penuh.\n");
        return (size_t)-1;
    }

    size_t offset = a->base + a->jumlah * sizeof(Order);
    Order *o = (Order *) arena_get(a->ar, offset);
    if (o == NULL) return (size_t)-1;

    o->id     = id;
    o->jumlah = jumlah;
    strncpy(o->nama, nama ? nama : "", 49);
    o->nama[49] = '\0';
    a->jumlah++;

    return offset;
}

// Akses elemen ke-i lewat kalkulasi offset langsung
Order *get_order(OrderArray *a, size_t i) {
    if (a == NULL || i >= a->jumlah) return NULL;
    size_t offset = a->base + i * sizeof(Order);
    return (Order *) arena_get(a->ar, offset);
}

// Tampilkan seluruh isi array beserta offset tiap elemen
void print_orders(OrderArray *a) {
    if (a == NULL) return;

    printf("\n=== DATABASE PESANAN (ARRAY) ===\n");
    printf("Kapasitas: %zu | Terisi: %zu | Base: %zu\n",
           a->kapasitas, a->jumlah, a->base);
    printf("--------------------------------\n");

    for (size_t i = 0; i < a->jumlah; i++) {
        size_t  off = a->base + i * sizeof(Order);
        Order  *o   = get_order(a, i);
        if (o)
            printf("[%zu] off=%-4zu | ID=%-3d | %-20s | jumlah=%d\n",
                   i, off, o->id, o->nama, o->jumlah);
    }
    printf("================================\n\n");
}