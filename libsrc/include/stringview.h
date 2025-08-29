#ifndef STRINGVIEW_H__
#define STRINGVIEW_H__

#include "types.h"
#include "str.h"

stringview_t stringview_create(const string_t* string, size_t start_idx, size_t count);
bool stringview_reset(stringview_t* stringview);

NODISCARD
char* stringview_cstr(const stringview_t* stringview);
NODISCARD
string_t stringview_owning(const stringview_t* stringview);
string_t stringview_as_string(const stringview_t* stringview);
size_t stringview_to_buffer(const stringview_t* stringview, char* buffer, size_t buffer_len);
size_t stringview_length(const stringview_t* stringview);

bool stringview_output(FILE* fd, const stringview_t* stringview);
#define stringview_print(STRV_PTR) stringview_output(stdout, (STRV_PTR))

int stringview_atoi(const stringview_t* stringview);
long stringview_atol(const stringview_t* stringview);
long long stringview_atoll(const stringview_t* stringview);
size_t stringview_atos(const stringview_t* stringivew);

bool stringview_source_compare(const stringview_t* a, const stringview_t* b);
bool stringview_value_compare(const stringview_t* a, const stringview_t* b);

bool stringview_compare_cstr(const stringview_t* view, const char* cstr);

bool stringview_is_palindrome(const stringview_t* view);

#endif // STRINGVIEW_H__
