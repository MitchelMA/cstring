#include <stdlib.h>
#include <stdint.h>
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

    size_t byte_count = sizeof(char) * stringview->count;
    uintptr_t start_addr = (uintptr_t)stringview->str_->text_ + sizeof(char) * stringview->start_idx;

    char* copy = malloc(byte_count + 1);
    if(copy == NULL)
        return NULL;

    memcpy(copy, (void*) start_addr, byte_count);
    copy[stringview->count] = '\0';
    return copy;
}

string_t stringview_owning(const stringview_t* stringview)
{
    NULL_CHECK(stringview, string_empty);

    size_t byte_count = sizeof(char) * stringview->count;
    uintptr_t start_addr = (uintptr_t) stringview->str_->text_ + sizeof(char) * stringview->start_idx;

    char* txt = malloc(byte_count);
    if(txt == NULL)
        return string_empty;

    memcpy(txt, (void*) start_addr, byte_count);
    return (string_t){stringview->count, txt};
}

size_t stringview_length(const stringview_t* stringview)
{
    NULL_CHECK(stringview, 0);
    return stringview->count;
}

bool stringview_output(FILE* fd, const stringview_t* stringview)
{
    NULL_CHECK(stringview, false);

    size_t count = stringview->count;
    char* data = stringview->str_->text_ + stringview->start_idx;

    fprintf(fd, "%.*s", count, data);

    return true;
}

int stringview_atoi(const stringview_t* stringview)
{
    NULL_CHECK(stringview, 0);

    int value =  0;
    int sign =   1;
    size_t idx = 0;
    char* chars = (stringview->str_->text_ + sizeof(char) * stringview->start_idx);

    ATON(stringview, int, value, sign, idx, chars);

    return value * sign;
}

long stringview_atol(const stringview_t* stringview)
{
    NULL_CHECK(stringview, 0);

    long value = 0;
    int sign = 1;
    size_t idx = 0;
    char* chars = (stringview->str_->text_ + sizeof(char) * stringview->start_idx);

    ATON(stringview, long, value, sign, idx, chars);

    return value * sign;
}

long long stringview_atoll(const stringview_t* stringview)
{
    NULL_CHECK(stringview, 0);

    long long value = 0;
    int sign = 1;
    size_t idx = 0;
    char* chars = (stringview->str_->text_ + sizeof(char) * stringview->start_idx);

    ATON(stringview, long long, value, sign, idx, chars);

    return value * sign;
}

size_t stringview_atos(const stringview_t* stringview)
{
    NULL_CHECK(stringview, 0);

    size_t value = 0;
    int sign = 0;
    size_t idx = 0;
    char* chars = (stringview->str_->text_ + sizeof(char) * stringview->start_idx);

    ATON(stringview, size_t, value, sign, idx, chars);
    UNUSED(sign);
    return value;
}

bool stringview_source_compare(const stringview_t* a, const stringview_t* b)
{
    NULL_CHECK(a, false);
    NULL_CHECK(b, false);

    if (a->str_ != b->str_)
        return false;

    if (a->start_idx != b->start_idx)
        return false;

    return a->count == b->count;
}

bool stringview_value_compare(const stringview_t* a, const stringview_t* b)
{
    NULL_CHECK(a, false);
    NULL_CHECK(b, false);

    if (a->count != b->count)
        return false;

    return strncmp(
        (char*)(a->str_->text_ + a->start_idx),
        (char*)(b->str_->text_ + b->start_idx),
        a->count
    ) == 0;
}

bool stringview_compare_cstr(const stringview_t* view, const char* cstr)
{
    NULL_CHECK(view, false);
    if (cstr == NULL)
        return false;

    size_t clen = strlen(cstr);

    if(view->count != clen)
        return false;

    printf("\"%s\" : \"", cstr);
    stringview_print(view);
    printf("\"\n");

    return strncmp(
        (char*)(view->str_->text_ + view->start_idx),
        cstr,
        view->count 
    ) == 0;
}
