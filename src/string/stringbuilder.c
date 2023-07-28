#include <stdlib.h>
#include <string.h>

#include "stringbuilder.h"

#define NULL_CHECK(BUILDER_PTR, RETURN)                              \
do {                                                                 \
    if((BUILDER_PTR) == NULL || (BUILDER_PTR)->char_vector_ == NULL) \
        return (RETURN);                                             \
} while(0)

#define NULL_CHECK_EMPTY(BUILDER_PTR)                                \
do {                                                                 \
    if((BUILDER_PTR) == NULL || (BUILDER_PTR)->char_vector_ == NULL) \
        return;                                                      \
} while(0)

#define NUMTOA(BUILDERPTR, NUM)                                      \
do                                                                   \
{                                                                    \
    stringbuilder_push_ch((BUILDERPTR), (char)((NUM) % 10) + '0'); \
} while(((NUM) /= 10) > 0)

stringbuilder_t stringbuilder_create(void)
{
    stringbuilder_t builder = {0};
    builder.char_vector_ = vector_create(sizeof(char));
    return builder;
}

bool stringbuilder_clean(stringbuilder_t* str_builder)
{
    NULL_CHECK(str_builder, false);

    vector_clean(str_builder->char_vector_);
    str_builder->char_vector_ = NULL;
    return true;
}

bool stringbuilder_append_ch(stringbuilder_t* str_builder, char character)
{
    NULL_CHECK(str_builder, false);

    vector_append(str_builder->char_vector_, (void*) &character);
    return true;
}

bool stringbuilder_append_cstr(stringbuilder_t* str_builder, const char* cstr)
{
    NULL_CHECK(str_builder, false);

    const size_t len = strlen(cstr);
    for(size_t i = 0; i < len; i++)
        vector_append(str_builder->char_vector_, (void*) (cstr + i));

    return true;
}

bool stringbuilder_append_str(stringbuilder_t* str_builder, const string_t* string)
{
    NULL_CHECK(str_builder, false);
    for(size_t i = 0; i < string->count_; i++)
        vector_append(str_builder->char_vector_, (void*) (string->text_ + i));

    return true;
}

bool stringbuilder_push_ch(stringbuilder_t* str_builder, char character)
{
    NULL_CHECK(str_builder, false);

    vector_push(str_builder->char_vector_, (void*) &character);
    return true;
}

char* stringbuilder_build_cstr(const stringbuilder_t* str_builder)
{
    NULL_CHECK(str_builder, NULL);

    size_t count = vector_get_elem_count(str_builder->char_vector_);

    char* copy = malloc(count + 1);
    memcpy(copy, vector_get_start_addr_(str_builder->char_vector_), count);
    copy[count] = '\0';
    return copy;
}

string_t stringbuilder_build(const stringbuilder_t* str_builder)
{
    NULL_CHECK(str_builder, (string_t){0});

    size_t count = vector_get_elem_count(str_builder->char_vector_);

    char* txt = malloc(count);
    memcpy(txt, vector_get_start_addr_(str_builder->char_vector_), count);
    string_t str = {count, txt};
    return str;
}

void stringbuilder_itoa(stringbuilder_t* str_builder, int value)
{
    NULL_CHECK_EMPTY(str_builder);
    stringbuilder_t tmp = stringbuilder_create();

    NUMTOA(&tmp, value);
    vector_append_to(str_builder->char_vector_, tmp.char_vector_, 0, 0);
    stringbuilder_clean(&tmp);
}

void stringbuilder_ltoa(stringbuilder_t* str_builder, long value)
{
    NULL_CHECK_EMPTY(str_builder);
    stringbuilder_t tmp = stringbuilder_create();

    NUMTOA(&tmp, value);
    vector_append_to(str_builder->char_vector_, tmp.char_vector_, 0, 0);
    stringbuilder_clean(&tmp);
}

void stringbuilder_lltoa(stringbuilder_t* str_builder, long long value)
{
    NULL_CHECK_EMPTY(str_builder);
    stringbuilder_t tmp = stringbuilder_create();

    NUMTOA(&tmp, value);
    vector_append_to(str_builder->char_vector_, tmp.char_vector_, 0, 0);
    stringbuilder_clean(&tmp);
}

void stringbuilder_stoa(stringbuilder_t* str_builder, size_t value)
{
    NULL_CHECK_EMPTY(str_builder);
    stringbuilder_t tmp = stringbuilder_create();

    NUMTOA(&tmp, value);
    vector_append_to(str_builder->char_vector_, tmp.char_vector_, 0, 0);
    stringbuilder_clean(&tmp);
}