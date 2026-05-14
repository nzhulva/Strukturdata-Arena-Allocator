#ifndef ORDER_ARRAY_H
#define ORDER_ARRAY_H

#include "arena.h"

// Satu record pesanan sembako
typedef struct {
    int  id;
    char nama[50];  // nama produk sembako
    int  jumlah;
} Order;

// Control struct array (hidup di luar arena)
// Menyimpan metadata: di mana data dimulai, kapasitas, jumlah terisi
typedef struct {
    Arena  *ar;
    size_t  base;       // offset awal array di arena
    size_t  kapasitas;  // kapasitas maksimum
    size_t  jumlah;     // jumlah data yang sudah masuk
} OrderArray;

int     create_array  (OrderArray *a, Arena *ar, size_t kapasitas);
size_t  insert_order  (OrderArray *a, int id, const char *nama, int jumlah);
Order  *get_order     (OrderArray *a, size_t i);
void    print_orders  (OrderArray *a);

#endif