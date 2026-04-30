#ifndef ARENA_H
#define ARENA_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t *memory;
    size_t total;
    size_t used;
} Arena;

void arena_init(Arena *ar, uint8_t *mem, size_t size);
size_t arena_alloc(Arena *ar, size_t req);
void* arena_get(Arena *ar, size_t offset);
void arena_reset(Arena *ar);
void arena_status(Arena *ar);

#endif