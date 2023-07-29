#ifndef STRINGBUILDER_H__
#define STRINGBUILDER_H__

#include <stdio.h>

#include <vector.h>
#include "../common.h"
#include "str.h"

typedef struct
{
    vector_t char_vector_;
} stringbuilder_t;

NODISCARD
stringbuilder_t stringbuilder_create(void);
bool stringbuilder_clean(stringbuilder_t* str_builder);

bool stringbuilder_append_ch(stringbuilder_t* str_builder, char character);
bool stringbuilder_append_cstr(stringbuilder_t* str_builder, const char* cstr);
bool stringbuilder_append_str(stringbuilder_t* str_builder, const string_t* string);

bool stringbuilder_push_ch(stringbuilder_t* str_builder, char character);

NODISCARD
char* stringbuilder_build_cstr(const stringbuilder_t* str_builder);
NODISCARD
string_t stringbuilder_build(const stringbuilder_t* str_builder);

void stringbuilder_itoa(stringbuilder_t* str_builder, int value);
void stringbuilder_ltoa(stringbuilder_t* str_builder, long value);
void stringbuilder_lltoa(stringbuilder_t* str_builder, long long value);
void stringbuilder_stoa(stringbuilder_t* stringbuilder, size_t value);

// Todo! Testing of the stringbuilder on windows and with files instead of only the stdin

bool stringbuilder_read(FILE* fstream, stringbuilder_t* str_builder);
#define stringbuilder_read_console(STR_BUILDER_PTR) stringbuilder_read(stdin, (STR_BUILDER_PTR))

#endif // STRINGBUILDER_H__