#ifndef STR_H__
#define STR_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <vector.h>

#include "types.h"
#include "../private/private_macros.h"

extern string_t string_empty;

NODISCARD
string_t string_create(const char* c_str);
bool string_clean(string_t* string);
size_t string_length(const string_t* string);
NODISCARD
string_t string_copy(const string_t* src);
#define string_add(...) string_add_(__VA_ARGS__, NULL)
NODISCARD
char* string_cstr(const string_t* string);
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

bool string_compare(const string_t* str1, const string_t* str2);
bool string_is_palindrome(const string_t* string);

int64_t string_find_cstr(const string_t* string, const char* match, int occurrence_index);

NODISCARD
/**
 *  Returns a vector with stringviews.
 *  @return A vector consisting of stringviews to the original string.
 *  On failure it returns NULL.
 */
vector_t* string_split(const string_t* string, const char* delim);

/**
 * Splits the string for `output_size` count
 * @param string The string that get's split
 * @param delim The delimiter
 * @param output The output array of size `output_size` that contains the delimitted values
 * @param output_size The size of `output`
 * @return The amount of processed delimitted values
 */
size_t string_split_static(const string_t* string, const char* delim, stringview_t* output, size_t output_size);

void string_sort_alpha(string_t* string);
void string_sort_alpha_inverse(string_t* string);

NODISCARD
string_t string_remove_match(const string_t* string, const char* match);
NODISCARD
string_t string_replace_match(const string_t* string, const char* match, const char* replace);

stringview_t string_trim_start(const string_t* string, const char* remove);
stringview_t string_trim_end(const string_t* string, const char* remove);
stringview_t string_trim_both(const string_t* string, const char* remove);

NODISCARD
string_t string_add_(const string_t* str1, ...);

#endif // STR_H__
