#ifndef STR_H__
#define STR_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "../common.h"

typedef struct 
{
    size_t count_;
    char* text_;
} string_t;

NODISCARD
string_t string_create(const char* c_str);
bool string_clean(string_t* string);
#define string_add(...) string_add_(__VA_ARGS__, NULL)
NODISCARD
char* string_c_str(const string_t* string);
bool string_output(FILE* fstream, const string_t* string);
#define string_print(STR_PTR) string_output(stdout, (STR_PTR))

NODISCARD
string_t string_read(FILE* fstream, size_t max_len);

NODISCARD
string_t string_itoa(int value);
NODISCARD
string_t string_ltoa(long value);
NODISCARD
string_t string_lltoa(long long value);
NODISCARD
string_t string_stoa(size_t value);

int string_atoi(const string_t* string);
long string_atol(const string_t* string);
long long string_atoll(const string_t* string);
size_t string_atos(const string_t* string);

NODISCARD
string_t string_add_(const string_t* str1, ...);

#endif // STR_H__