#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stringview.h"
#include "stringbuilder.h"

#define NULL_CHECK(STR_V_PTR, RETURN)                                               \
do                                                                                  \
{                                                                                   \
    if((STR_V_PTR) == NULL || (STR_V_PTR)->str_ == NULL || (STR_V_PTR)->count == 0) \
        return (RETURN);                                                            \
} while(0)

#define NULL_CHECK_EMPTY(STR_V_PTR)                                                 \
do                                                                                  \
{                                                                                   \
    if((STR_V_PTR) == NULL || (STR_V_PTR)->str_ == NULL || (STR_V_PTR)->count == 0) \
        return;                                                                     \
} while(0)

#define NULL_CHECK_STR(STR_PTR, RETURN)                                         \
do                                                                              \
{                                                                               \
    if((STR_PTR) == NULL || (STR_PTR)->text_ == NULL || (STR_PTR)->count_ == 0) \
        return (RETURN);                                                        \
} while(0)

#define ATON(STRINGVIEWPTR, TYPE, VALUE, SIGN, IDX, CHARS)     \
do                                                             \
{                                                              \
    if(*(CHARS) == '+' || *(CHARS) == '-')                     \
    {                                                          \
        if(*(CHARS)++ == '-') (SIGN) = -1;                     \
        (IDX)++;                                               \
    }                                                          \
                                                               \
    while((IDX) < (STRINGVIEWPTR)->count && isdigit(*(CHARS))) \
    {                                                          \
        (VALUE) *= 10;                                         \
        (VALUE) += (TYPE) (*(CHARS)++ - '0');                  \
        (IDX)++;                                               \
    }                                                          \
} while(0)

//Todo! Be more pedantic with `sizeof(char)` operator. Currently not used consistently!

stringview_t stringview_create(const string_t* string, size_t start_idx, size_t count)
{
    NULL_CHECK_STR(string, (stringview_t){0});

    if(count == 0)
        count = string->count_ - start_idx;

    if((start_idx + count) > string->count_)
        return (stringview_t){0};

    stringview_t strview = {start_idx, count, string};
    return strview;
}

bool stringview_reset(stringview_t* stringview)
{
    NULL_CHECK(stringview, false);

    stringview->start_idx = 0;
    stringview->count = 0;
    // just set it to NULL: stringview is *NOT* the owner of the char-array/string_t
    stringview->str_ = NULL;
    return true;
}

char* stringview_cstr(const stringview_t* stringview)
{
    NULL_CHECK(stringview, NULL);

    char* copy = malloc(stringview->count + 1);
    memcpy(copy, stringview->str_->text_ + stringview->start_idx, stringview->count);
    copy[stringview->count] = '\0';
    return copy;
}

bool stringview_output(FILE* fd, const stringview_t* stringview)
{
    NULL_CHECK(stringview, false);

    stringbuilder_t format_builder = stringbuilder_create();
    stringbuilder_append_cstr(&format_builder, "%.");
    stringbuilder_stoa(&format_builder, stringview->count);
    stringbuilder_append_ch(&format_builder, 's');
    char* cstr_format = stringbuilder_build_cstr(&format_builder);
    stringbuilder_clean(&format_builder);
    fprintf(fd, cstr_format, (stringview->str_->text_ + stringview->start_idx));
    free(cstr_format);

    return true;
}

int stringview_atoi(const stringview_t* stringview)
{
    NULL_CHECK(stringview, 0);

    int value =  0;
    int sign =   1;
    size_t idx = 0;
    char* chars = (stringview->str_->text_ + stringview->start_idx);

    ATON(stringview, int, value, sign, idx, chars);

    return value * sign;
}

long stringview_atol(const stringview_t* stringview)
{
    NULL_CHECK(stringview, 0);

    long value = 0;
    int sign = 1;
    size_t idx = 0;
    char* chars = (stringview->str_->text_ + stringview->start_idx);

    ATON(stringview, long, value, sign, idx, chars);

    return value * sign;
}

long long stringview_atoll(const stringview_t* stringview)
{
    NULL_CHECK(stringview, 0);

    long long value = 0;
    int sign = 1;
    size_t idx = 0;
    char* chars = (stringview->str_->text_ + stringview->start_idx);

    ATON(stringview, long long, value, sign, idx, chars);

    return value * sign;
}

size_t stringview_atos(const stringview_t* stringview)
{
    NULL_CHECK(stringview, 0);

    size_t value = 0;
    int sign = 0;
    size_t idx = 0;
    char* chars = (stringview->str_->text_ + stringview->start_idx);

    ATON(stringview, size_t, value, sign, idx, chars);
    UNUSED(sign);
    return value;
}