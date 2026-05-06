#include <stdio.h>
#include <string.h>
#include "queue.h"

// Inisialisasi antrian kosong
void queue_init(Queue *q, Arena *ar) {
    q->ar    = ar;
    q->front = -1;  // belum ada node depan
    q->rear  = -1;  // belum ada node belakang
    q->size  = 0;
}

// Kembalikan 1 jika antrian kosong, 0 jika ada isi
int queue_is_empty(Queue *q) {
    return q->front == -1;
}

// Masukkan pesanan ke belakang antrian
int enqueue(Queue *q, int id, const char *nama, int jumlah) {

    // alokasi node baru di arena
    size_t offset = arena_alloc(q->ar, sizeof(QueueNode));
    if (offset == (size_t)-1) return -1;

    // ambil pointer ke node dari arena
    QueueNode *node = (QueueNode*) arena_get(q->ar, offset);
    if (node == NULL) return -1;

    // isi data pesanan
    node->id = id;
    strncpy(node->nama, nama, 49);
    node->nama[49] = '\0';
    node->jumlah = jumlah;
    node->next   = -1;  // node baru selalu di paling belakang

    // hubungkan ke node sebelumnya (jika ada)
    if (q->rear != -1) {
        QueueNode *last = (QueueNode*) arena_get(q->ar, q->rear);
        last->next = (int)offset;
    }

    // update rear ke node baru
    q->rear = (int)offset;

    // kalau antrian tadinya kosong, front ikut diset
    if (q->front == -1) {
        q->front = (int)offset;
    }

    q->size++;
    printf("Pesanan masuk antrian: [ID=%d | %s | jumlah=%d] di offset=%zu\n",
           id, nama, jumlah, offset);
    return (int)offset;
}

// Keluarkan pesanan dari depan antrian (FIFO)
// Data pesanan yang keluar disimpan ke out_id, out_nama, out_jumlah
int dequeue(Queue *q, int *out_id, char *out_nama, int *out_jumlah) {

    // cek antrian kosong
    if (queue_is_empty(q)) {
        printf("GAGAL: antrian kosong, tidak ada pesanan.\n");
        return -1;
    }

    // ambil node terdepan
    QueueNode *node = (QueueNode*) arena_get(q->ar, q->front);
    if (node == NULL) return -1;

    // salin data keluar (jika pointer output tersedia)
    if (out_id)     *out_id     = node->id;
    if (out_nama)   strncpy(out_nama, node->nama, 49);
    if (out_jumlah) *out_jumlah = node->jumlah;

    printf("Pesanan keluar (FIFO): [ID=%d | %s | jumlah=%d] dari offset=%d\n",
           node->id, node->nama, node->jumlah, q->front);

    // geser front ke node berikutnya
    q->front = node->next;

    // kalau sudah habis, reset rear juga
    if (q->front == -1) {
        q->rear = -1;
    }

    q->size--;
    return 0;
}

// Tampilkan semua isi antrian dari depan ke belakang (traversal)
void queue_print(Queue *q) {

    printf("\n=== ANTRIAN PESANAN (depan -> belakang) ===\n");

    if (queue_is_empty(q)) {
        printf("(antrian kosong)\n");
        printf("===========================================\n\n");
        return;
    }

    printf("Jumlah pesanan: %d\n", q->size);

    int current = q->front;
    int urutan  = 1;

    while (current != -1) {
        QueueNode *node = (QueueNode*) arena_get(q->ar, current);
        printf("[%d] offset=%d | ID=%d | %s | jumlah=%d\n",
               urutan, current, node->id, node->nama, node->jumlah);
        current = node->next;
        urutan++;
    }

    printf("===========================================\n\n");
}