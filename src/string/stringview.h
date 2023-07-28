#ifndef STRINGVIEW_H__
#define STRINGVIEW_H__

#include "str.h"

typedef struct
{
    size_t start_idx;
    size_t count;
    const string_t* str_;
} stringview_t;

stringview_t stringview_create(const string_t* string, size_t start_idx, size_t count);
bool stringview_reset(stringview_t* stringview);

NODISCARD
char* stringview_cstr(const stringview_t* stringview);

void stringview_output(FILE* fd, const stringview_t* stringview);
#define stringview_print(STRV_PTR) stringview_output(stdout, (STRV_PTR))

#endif // STRINGVIEW_H__