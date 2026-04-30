#ifndef ORDER_ARRAY_H
#define ORDER_ARRAY_H

#include "arena.h"

// Struct data pesanan (satu record)
typedef struct {
    int id;             // ID pesanan
    char nama[50];      // nama item / pelanggan
    int jumlah;         // jumlah item
} Order;

// Struct array yang hidup di dalam arena
typedef struct {
    Arena *ar;          // arena tempat data disimpan
    size_t base;        // offset awal array di arena
    size_t kapasitas;   // kapasitas maksimum
    size_t jumlah;      // jumlah data saat ini
} OrderArray;

// Membuat array di arena
int create_array(OrderArray *a, Arena *ar, size_t kapasitas);

// Menambahkan data pesanan
size_t insert_order(OrderArray *a, int id, const char *nama, int jumlah);

// Mengambil data berdasarkan index
Order* get_order(OrderArray *a, size_t i);

// Menampilkan seluruh data
void print_orders(OrderArray *a);

#endif