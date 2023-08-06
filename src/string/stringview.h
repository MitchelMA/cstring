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
NODISCARD
string_t stringview_owning(const stringview_t* stringview);

bool stringview_output(FILE* fd, const stringview_t* stringview);
#define stringview_print(STRV_PTR) stringview_output(stdout, (STRV_PTR))

int stringview_atoi(const stringview_t* stringview);
long stringview_atol(const stringview_t* stringview);
long long stringview_atoll(const stringview_t* stringview);
size_t stringview_atos(const stringview_t* stringivew);

bool stringview_compare_cstr(const stringview_t* str_view, const char* cstr);

#endif // STRINGVIEW_H__
