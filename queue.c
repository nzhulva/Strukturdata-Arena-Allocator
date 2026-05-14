#include <stdio.h>
#include <string.h>
#include "queue.h"

// Siapkan antrian kosong
void queue_init(Queue *q, Arena *ar) {
    q->ar    = ar;
    q->front = -1;
    q->rear  = -1;
    q->size  = 0;
}

int queue_is_empty(Queue *q) {
    return q->front == -1;
}

// Tambah pesanan ke belakang antrian (enqueue)
int enqueue(Queue *q, int id, const char *nama, int jumlah) {
    size_t offset = arena_alloc(q->ar, sizeof(QueueNode));
    if (offset == (size_t)-1) return -1;

    QueueNode *node = (QueueNode *) arena_get(q->ar, offset);
    if (node == NULL) return -1;

    node->id     = id;
    node->jumlah = jumlah;
    node->next   = -1;
    strncpy(node->nama, nama ? nama : "", 49);
    node->nama[49] = '\0';

    // sambungkan rear lama ke node baru
    if (q->rear != -1) {
        QueueNode *last = (QueueNode *) arena_get(q->ar, q->rear);
        last->next = (int) offset;
    }
    q->rear = (int) offset;

    if (q->front == -1)
        q->front = (int) offset;

    q->size++;
    printf("  Pesanan Live TikTok masuk: [ID=%-3d | %-20s | x%d] offset=%zu\n",
           id, nama ? nama : "", jumlah, offset);
    return (int) offset;
}

// Ambil pesanan dari depan antrian (dequeue — FIFO)
int dequeue(Queue *q, int *out_id, char *out_nama, int *out_jumlah) {
    if (queue_is_empty(q)) {
        printf("  [KOSONG] Tidak ada pesanan.\n");
        return -1;
    }

    QueueNode *node = (QueueNode *) arena_get(q->ar, q->front);
    if (node == NULL) return -1;

    if (out_id)     *out_id     = node->id;
    if (out_jumlah) *out_jumlah = node->jumlah;
    if (out_nama)   strncpy(out_nama, node->nama, 49);

    printf("  [PROSES] ID=%-3d | %-20s | x%d | dari offset=%d\n",
           node->id, node->nama, node->jumlah, q->front);

    q->front = node->next;
    if (q->front == -1) q->rear = -1;
    q->size--;
    return 0;
}

// Tampilkan isi antrian dari depan ke belakang
void queue_print(Queue *q) {
    printf("\n=== ANTRIAN PESANAN (depan -> belakang) ===\n");

    if (queue_is_empty(q)) {
        printf("  (antrian kosong)\n===========================================\n\n");
        return;
    }

    printf("Jumlah: %d pesanan\n-------------------------------------------\n", q->size);

    int current = q->front;
    int urutan  = 1;
    while (current != -1) {
        QueueNode *node = (QueueNode *) arena_get(q->ar, current);
        printf("[%d] off=%-4d | ID=%-3d | %-20s | x%d\n",
               urutan, current, node->id, node->nama, node->jumlah);
        current = node->next;
        urutan++;
    }
    printf("===========================================\n\n");
}