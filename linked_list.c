#include <stdio.h>
#include <string.h>
#include "linked_list.h"

void ll_init(LinkedList *list, Arena *ar) {
    list->ar = ar;
    list->head = -1;
}

int ll_insert(LinkedList *list, int id, const char *nama, int jumlah) {

    size_t offset = arena_alloc(list->ar, sizeof(OrderNode));
    if (offset == (size_t)-1) return -1;

    OrderNode *node = (OrderNode*) arena_get(list->ar, offset);

    node->id = id;
    strncpy(node->nama, nama, 49);
    node->nama[49] = '\0';
    node->jumlah = jumlah;
    node->next = -1;

    if (list->head == -1) {
        list->head = (int)offset;
        return offset;
    }

    int current = list->head;

    while (1) {
        OrderNode *temp = (OrderNode*) arena_get(list->ar, current);
        if (temp->next == -1) break;
        current = temp->next;
    }

    OrderNode *last = (OrderNode*) arena_get(list->ar, current);
    last->next = (int)offset;

    return offset;
}

void ll_print(LinkedList *list) {

    printf("\n=== LINKED LIST PESANAN ===\n");

    int current = list->head;

    while (current != -1) {

        OrderNode *node = (OrderNode*) arena_get(list->ar, current);

        printf("Offset=%d | ID=%d | %s | jumlah=%d\n",
               current, node->id, node->nama, node->jumlah);

        current = node->next;
    }

    printf("NULL\n");
}