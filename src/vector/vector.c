//
// Created by Mitchel on 17-7-2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <errno.h>

#include "vector.h"

#define NULL_CHECK(PTR)              \
    do {                             \
        if((PTR) == NULL)            \
        {                            \
            perror(strerror(errno)); \
            exit(EXIT_FAILURE);      \
        }                            \
    } while(0)

#define INCREASE_CHECK_AM(VECTOR, AMOUNT)                                  \
    do {                                                                   \
        if((VECTOR)->elem_count + AMOUNT >= (VECTOR)->elem_capacity_count) \
            vec_double_cap((VECTOR));                                      \
    } while((VECTOR)->elem_count + AMOUNT > (VECTOR)->elem_capacity_count)

#define INCREASE_CHECK(VECTOR) \
    INCREASE_CHECK_AM(VECTOR, 1)

#define DECREASE_CHECK(VECTOR)                                                \
    do {                                                                      \
        if(((VECTOR)->elem_count - 1 <= (VECTOR)->elem_capacity_count / 2) && \
            (VECTOR)->elem_count - 1 >= VECTOR_START_CAPACITY)                \
            vec_half_cap((VECTOR));                                           \
    } while(0)


void vec_double_cap(struct vector_* vec);

void vec_half_cap(struct vector_* vec);

size_t calc_capacity_(size_t elem_count);

struct vector_
{
    size_t elem_count;
    size_t elem_capacity_count;
    size_t elem_size;
    void* start_addr;
};

vector_t* vector_create(size_t elem_size)
{
    vector_t* vec = malloc(sizeof(vector_t));
    NULL_CHECK(vec);
    vec->elem_count = 0;
    vec->elem_size = elem_size;
    vec->elem_capacity_count = VECTOR_START_CAPACITY;
    vec->start_addr = malloc(vec->elem_capacity_count * vec->elem_size);
    return vec;
}

vector_t* vector_create_from(size_t elem_size, size_t elem_count, const void* start_addr)
{
    vector_t* vec = malloc(sizeof(vector_t));
    NULL_CHECK(vec);
    vec->elem_count = elem_count;
    vec->elem_size = elem_size;
    vec->elem_capacity_count = calc_capacity_(elem_count);
    vec->start_addr = malloc(vec->elem_capacity_count * vec->elem_size);
    memcpy(vec->start_addr, start_addr, elem_size * elem_count);
    return vec;
}

void vector_clean(vector_t* vec)
{
    free(vec->start_addr);
    free(vec);
}

size_t vector_get_elem_count(const vector_t* vec)
{
    return vec->elem_count;
}

size_t vector_get_capacity(const vector_t* vec)
{
    return vec->elem_capacity_count;
}

size_t vector_get_elem_size(const vector_t* vec)
{
    return vec->elem_size;
}

void* vector_get_start_addr_(const vector_t* vec)
{
    return vec->start_addr;
}

bool vector_elem_at(const vector_t* vec, size_t index, void** out)
{
    if (index >= vec->elem_count)
        return false;

    uintptr_t addr = (uintptr_t) vec->start_addr + index * vec->elem_size;
    *out = (void*) addr;
    return true;
}

bool vector_elem_at_ptr(const vector_t* vec, size_t index, void* out)
{
    uintptr_t* addr_holder;
    if (!vector_elem_at(vec, index, (void**) &addr_holder))
        return false;

    memcpy(out, addr_holder, sizeof(uintptr_t));
    return true;
}

void vector_push(vector_t* vec, void* elem)
{
    INCREASE_CHECK(vec);
    uintptr_t target_move_addr = (uintptr_t) vec->start_addr + vec->elem_size;
    size_t bytes_to_move = vec->elem_count * vec->elem_size;
    memmove((void*) target_move_addr, vec->start_addr, bytes_to_move);
    memcpy(vec->start_addr, elem, vec->elem_size);
    vec->elem_count++;
}

void vector_append(vector_t* vec, void* elem)
{
    INCREASE_CHECK(vec);
    uintptr_t addr = (uintptr_t) vec->start_addr + vec->elem_count * vec->elem_size;
    memcpy((void*) addr, elem, vec->elem_size);
    vec->elem_count++;
}

bool vector_insert_to(vector_t* vec, size_t index, void* elem)
{
    if (index == 0)
    {
        vector_push(vec, elem);
        return true;
    }

    if (index == vec->elem_count)
    {
        vector_append(vec, elem);
        return true;
    }

    if (index >= vec->elem_count)
        return false;

    INCREASE_CHECK(vec);

    uintptr_t target_ins_addr = (uintptr_t) vec->start_addr + vec->elem_size * index;
    uintptr_t end_addr        = (uintptr_t) vec->start_addr + vec->elem_count * vec->elem_size;
    size_t bytes_to_move      = end_addr - target_ins_addr;

    memmove((void*) (target_ins_addr + vec->elem_size), (void*) target_ins_addr, bytes_to_move);
    memcpy((void*) target_ins_addr, elem, vec->elem_size);
    vec->elem_count++;
    return true;
}

vector_t* vector_copy(const vector_t* vec, size_t start_index, size_t elem_count)
{
    if(elem_count == 0)
        elem_count = vec->elem_count - start_index;

    if ((elem_count + start_index) > vec->elem_count)
        return NULL;

    uintptr_t start_copy_addr = (uintptr_t) vec->start_addr + start_index * vec->elem_size;
    size_t amount_to_copy     = elem_count * vec->elem_size;

    vector_t* ret = malloc(sizeof(vector_t));
    NULL_CHECK(ret);
    ret->elem_capacity_count = calc_capacity_(elem_count);
    ret->elem_count = elem_count;
    ret->elem_size = vec->elem_size;
    ret->start_addr = malloc(ret->elem_capacity_count * ret->elem_size);

    memcpy(ret->start_addr, (void*) start_copy_addr, amount_to_copy);

    return ret;
}

bool vector_copy_into(vector_t* dst, size_t dst_index, const vector_t* src, size_t src_index,
                      size_t elem_count)
{
    if(elem_count == 0)
        elem_count = src->elem_count - src_index;

    if (
            elem_count == 0 ||
            dst_index > dst->elem_count ||
            (elem_count + src_index) > src->elem_count ||
            src->elem_size != dst->elem_size)
        return false;

    INCREASE_CHECK_AM(dst, elem_count);

    uintptr_t src_copy_addr   = (uintptr_t) src->start_addr + src_index * src->elem_size;
    uintptr_t dst_insert_addr = (uintptr_t) dst->start_addr + dst_index * dst->elem_size;
    uintptr_t dst_end_addr    = (uintptr_t) dst->start_addr + dst->elem_count * dst->elem_size;
    size_t bytes_to_move      = dst_end_addr - dst_insert_addr;
    size_t bytes_to_copy      = elem_count * src->elem_size;

    memmove((void*) (dst_insert_addr + bytes_to_copy), (void*) dst_insert_addr, bytes_to_move);
    memcpy((void*) dst_insert_addr, (void*) src_copy_addr, bytes_to_copy);
    dst->elem_count += elem_count;

    return true;
}

bool vector_pop(vector_t* vec, void* out)
{
    if (vec->elem_count == 0)
        return false;

    if (out != NULL)
        memcpy(out, vec->start_addr, vec->elem_size);


    uintptr_t target_addr = (uintptr_t) vec->start_addr + vec->elem_size;
    size_t bytes_to_move  = (vec->elem_count - 1) * vec->elem_size;
    memmove(vec->start_addr, (void*) target_addr, bytes_to_move);

    DECREASE_CHECK(vec);
    vec->elem_count--;
    return true;
}

bool vector_dequeue(vector_t* vec, void* out)
{
    if (vec->elem_count == 0)
        return false;

    uintptr_t end_addr = (uintptr_t) vec->start_addr + vec->elem_count * vec->elem_size;
    if (out != NULL)
        memcpy(out, (void*) (end_addr - vec->elem_size), vec->elem_size);

    DECREASE_CHECK(vec);
    vec->elem_count--;
    return true;
}

bool vector_remove_at(vector_t* vec, size_t index, void* out)
{
    if (index == 0)
        return vector_pop(vec, out);

    if (index == vec->elem_count - 1)
        return vector_dequeue(vec, out);

    if (index >= vec->elem_count)
        return false;

    uintptr_t target_rem_addr = (uintptr_t) vec->start_addr + vec->elem_size * index;
    uintptr_t end_addr        = (uintptr_t) vec->start_addr + (vec->elem_count - 1) * vec->elem_size;
    size_t bytes_to_move      = end_addr - target_rem_addr;

    if (out != NULL)
        memcpy(out, (void*) target_rem_addr, vec->elem_size);

    memmove((void*) target_rem_addr, (void*) (target_rem_addr + vec->elem_size), bytes_to_move);
    DECREASE_CHECK(vec);
    vec->elem_count--;
    return true;
}

void vector_add_with_(vector_t* vec, void(*method)(vector_t*, void*), ...)
{
    va_list args;
    va_start(args, method);
    uintptr_t ptr = va_arg(args, uintptr_t);

    while(ptr != 0)
    {
        method(vec, (void*)ptr);
        ptr = va_arg(args, uintptr_t);
    }

    va_end(args);
}

bool vector_insert_ex_(vector_t* vec, size_t start_index, ...)
{
    va_list args;
    va_start(args, start_index);
    uintptr_t ptr = va_arg(args, uintptr_t);

    while(ptr != 0)
    {
        if(!vector_insert_to(vec, start_index, (void*)ptr)) return false;
        start_index++;
        ptr = va_arg(args, uintptr_t);
    }

    va_end(args);
    return true;
}

void vec_double_cap(vector_t* vec)
{
    void* new_addr = realloc(vec->start_addr, vec->elem_size * vec->elem_capacity_count * 2);
    NULL_CHECK(new_addr);
    vec->elem_capacity_count *= 2;
    vec->start_addr = new_addr;
}

void vec_half_cap(vector_t* vec)
{
    void* new_addr = realloc(vec->start_addr, vec->elem_size * (vec->elem_capacity_count / 2));
    NULL_CHECK(new_addr);
    vec->elem_capacity_count /= 2;
    vec->start_addr = new_addr;
}

size_t calc_capacity_(size_t elem_count)
{
    size_t num = VECTOR_START_CAPACITY;
    while (num <= elem_count) num *= 2;
    return num;
}