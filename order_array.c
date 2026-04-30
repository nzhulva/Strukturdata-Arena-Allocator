#include <stdio.h>
#include <string.h>
#include "order_array.h"

// Membuat array baru di arena
int create_array(OrderArray *a, Arena *ar, size_t kapasitas) {

    // cek parameter biar aman
    if (a == NULL || ar == NULL || kapasitas == 0) {
        printf("ERROR: parameter tidak valid\n");
        return 0;
    }

    // alokasi memori di arena
    size_t pos = arena_alloc(ar, kapasitas * sizeof(Order));

    // cek kalau gagal
    if (pos == (size_t)-1) {
        printf("GAGAL: arena tidak cukup\n");
        return 0;
    }

    // isi struct array
    a->ar = ar;
    a->base = pos;
    a->kapasitas = kapasitas;
    a->jumlah = 0;

    printf("Array dibuat di offset %zu\n", pos);
    return 1;
}

// Menambahkan pesanan ke array
size_t insert_order(OrderArray *a, int id, const char *nama, int jumlah) {

    // validasi awal
    if (a == NULL || a->ar == NULL) {
        printf("ERROR: array belum siap\n");
        return (size_t)-1;
    }

    // cek penuh
    if (a->jumlah >= a->kapasitas) {
        printf("GAGAL: array penuh\n");
        return (size_t)-1;
    }

    // hitung offset posisi data
    size_t offset = a->base + a->jumlah * sizeof(Order);

    // ambil alamat dari arena
    Order *o = (Order*) arena_get(a->ar, offset);

    // cek kalau gagal akses
    if (o == NULL) {
        printf("ERROR: akses gagal\n");
        return (size_t)-1;
    }

    // isi data
    o->id = id;

    // amanin string
    if (nama != NULL) {
        strncpy(o->nama, nama, 49);
        o->nama[49] = '\0';
    } else {
        o->nama[0] = '\0';
    }

    o->jumlah = jumlah;

    // update jumlah data
    a->jumlah++;

    printf("Pesanan masuk di offset %zu\n", offset);
    return offset;
}

// Mengambil data berdasarkan index
Order* get_order(OrderArray *a, size_t i) {

    // validasi
    if (a == NULL || a->ar == NULL || i >= a->jumlah) {
        return NULL;
    }

    // hitung offset
    size_t offset = a->base + i * sizeof(Order);

    // ambil dari arena
    return (Order*) arena_get(a->ar, offset);
}

// Menampilkan seluruh isi array
void print_orders(OrderArray *a) {

    if (a == NULL) return;

    printf("\n=== DATA PESANAN ===\n");
    printf("Kapasitas: %zu | Terisi: %zu\n", a->kapasitas, a->jumlah);
    printf("Base offset: %zu\n", a->base);

    for (size_t i = 0; i < a->jumlah; i++) {

        // ambil offset
        size_t offset = a->base + i * sizeof(Order);

        // ambil data
        Order *o = get_order(a, i);

        if (o != NULL) {
            printf("[%zu] off=%zu | id=%d | %s | jumlah=%d\n",
                   i, offset, o->id, o->nama, o->jumlah);
        }
    }

    printf("====================\n\n");
}