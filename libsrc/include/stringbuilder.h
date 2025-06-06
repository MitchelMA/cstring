#ifndef STRINGBUILDER_H__
#define STRINGBUILDER_H__

#include <stdio.h>

#include <vector.h>
#include "../private/private_macros.h"
#include "types.h"
#include "str.h"
#include "stringview.h"


NODISCARD
stringbuilder_t stringbuilder_create(void);
NODISCARD
stringbuilder_t stringbuilder_create_from_cstr(const char* cstr);
NODISCARD
stringbuilder_t stringbuilder_create_from_str(const string_t* str);
NODISCARD
stringbuilder_t stringbuilder_create_from_strv(const stringview_t* strv);

bool stringbuilder_clean(stringbuilder_t* str_builder);
bool stringbuilder_reset(stringbuilder_t* str_builder);
size_t stringbuilder_length(const stringbuilder_t* str_builder);

bool stringbuilder_append_ch(stringbuilder_t* str_builder, char character);
bool stringbuilder_append_cstr(stringbuilder_t* str_builder, const char* cstr);
bool stringbuilder_append_str(stringbuilder_t* str_builder, const string_t* str);
bool stringbuilder_append_strv(stringbuilder_t* str_builder, const stringview_t* strv);

bool stringbuilder_push_ch(stringbuilder_t* str_builder, char character);
bool stringbuilder_push_cstr(stringbuilder_t* str_builder, const char* cstr);
bool stringbuilder_push_str(stringbuilder_t* str_builder, const string_t* str);
bool stringbuilder_push_strv(stringbuilder_t* str_builder, const stringview_t* strv);

bool stringbuilder_insert_ch(stringbuilder_t* str_builder, size_t idx, char character);
bool stringbuilder_insert_cstr(stringbuilder_t* str_builder, size_t idx, const char* cstr);
bool stringbuilder_insert_str(stringbuilder_t* str_builder, size_t idx, const string_t* str);
bool stringbuilder_insert_strv(stringbuilder_t* str_builder, size_t idx, const stringview_t* strv);

char* stringbuilder_char_at(const stringbuilder_t* str_builder, size_t idx);

char stringbuilder_pop(stringbuilder_t* str_builder);
char stringbuilder_dequeue(stringbuilder_t* str_builder);
char stringbuilder_removeat(stringbuilder_t* str_builder, size_t idx);

NODISCARD
char* stringbuilder_build_cstr(const stringbuilder_t* str_builder);
NODISCARD
string_t stringbuilder_build(const stringbuilder_t* str_builder);

void stringbuilder_itoa(stringbuilder_t* str_builder, int value);
void stringbuilder_ltoa(stringbuilder_t* str_builder, long value);
void stringbuilder_lltoa(stringbuilder_t* str_builder, long long value);
void stringbuilder_stoa(stringbuilder_t* str_builder, size_t value);

bool stringbuilder_read(FILE* fstream, stringbuilder_t* str_builder, size_t max_read_count);
#define stringbuilder_read_console(STR_BUILDER_PTR) stringbuilder_read(stdin, (STR_BUILDER_PTR), ~(size_t) 0)

bool stringbuilder_output(FILE* fstream, const stringbuilder_t* builder);
#define stringbuilder_print(STR_BUILDER_PTR) stringbuilder_output(stdout, (STR_BUILDER_PTR))

bool stringbuilder_compare(const stringbuilder_t* a, const stringbuilder_t* b);
bool stringbuilder_compare_string(const stringbuilder_t* builder, const string_t* str);
bool stringbuilder_compare_view(const stringbuilder_t* builder, const stringview_t* view);
bool stringbuilder_compare_cstr(const stringbuilder_t* builder, const char* cstr);


#endif // STRINGBUILDER_H__
