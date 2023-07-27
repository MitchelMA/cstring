#include <stdlib.h>
#include <string.h>

#include "stringbuilder.h"

#define NULL_CHECK(BUILDER_PTR, RETURN)                              \
do {                                                                 \
    if((BUILDER_PTR) == NULL || (BUILDER_PTR)->char_vector_ == NULL) \
        return (RETURN);                                             \
} while(0)

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

char* stringbuilder_build_cstr(const stringbuilder_t* str_builder)
{
    NULL_CHECK(str_builder, NULL);

    size_t count = vector_get_elem_count(str_builder->char_vector_);

    char* copy = malloc(count + 1);
    memcpy(copy, vector_get_start_addr_(str_builder->char_vector_), count);
    copy[count] = '\0';
    return copy;
}