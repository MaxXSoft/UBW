#include "../include/mem.h"

// size of heap memory that to be allocated each time
#define LEAST_ALLOC 128

// definition of linked list header
typedef int Align;
typedef union HeaderPrototype {
    struct {
        union HeaderPrototype *next;
        size_t size;
    } s;
    Align x;
} Header;

// head of linked list
static Header base;

// linked list that stores free memory blocks
static Header *free_unit = NULL;

// heap
uint8_t *heap_base = (uint8_t *)MEM_HEAP_BASE;

// function in C library
static uint8_t *sbrk(int size) {
    if ((uint32_t)heap_base + size > MEM_HEAP_BASE + MEM_HEAP_SIZE) {
        return NULL;
    }
    uint8_t *p = heap_base;
    heap_base += size;
    return p;
}

static Header *MoreCore(size_t length) {
    size_t unit_size;
    unit_size = LEAST_ALLOC * ((length + LEAST_ALLOC - 1) / LEAST_ALLOC);
    uint8_t *core_ptr = sbrk(unit_size * sizeof(Header));
    if (core_ptr == NULL) return NULL;
    Header *unit_ptr = (Header *)core_ptr;
    unit_ptr->s.size = unit_size;
    free(unit_ptr + 1);
    return free_unit;
}

void *malloc(size_t length) {
    Header *p, *prev;
    size_t unit_size = (length + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prev = free_unit) == NULL) {
        // no free blocks
        base.s.next = free_unit = prev = &base;
        base.s.size = 0;
    }
    for (p = prev->s.next; ; prev = p, p = p->s.next) {
        // big enough
        if (p->s.size >= unit_size) {
            if (p->s.size == unit_size) {
                // exactly
                prev->s.next = p->s.next;
            }
            else {
                // allocate tail end
                p->s.size -= unit_size;
                p += p->s.size;
                p->s.size = unit_size;
            }
            free_unit = prev;
            return p + 1;
        }
        if (p == free_unit) {
            if ((p = MoreCore(unit_size)) == NULL) {
                // no spare space
                return NULL;
            }
        }
    }
    return NULL;
}

void free(void *ptr) {
    // point to the header
    Header *p = (Header *)ptr - 1, *q;
    for (q = free_unit; !(p > q && p < q->s.next); q = q->s.next) {
        if (q >= q->s.next && (p > q || p < q->s.next)) break;
    }
    // merge with previous adjacent unit
    if (p + p->s.size == q->s.next) {
        p->s.size += q->s.next->s.size;
        p->s.next = q->s.next->s.next;
    }
    else {
        p->s.next = q->s.next;
    }
    // merge with next adjacent unit
    if (q + q->s.size == p) {
        q->s.size += p->s.size;
        q->s.next = p->s.next;
    }
    else {
        q->s.next = p;
    }
    // set the free unit pointer
    free_unit = q;
}

void *memset(void *dest, uint8_t byte, size_t length) {
    uint8_t *ptr = dest;
    while (length--) *ptr++ = byte;
    return dest;
}

void *memcpy(void *dest, const void *src, size_t length) {
    uint8_t *ptr = dest;
    while (length--) *ptr++ = *((uint8_t *)src++);
    return dest;
}

int memcmp(const void *lhs, const void *rhs, size_t length) {
    int result = 0;
    while (length--) {
        result = (int)(*((uint8_t *)lhs++)) - *((uint8_t *)rhs++);
        if (result) return result;
    }
    return result;
}
