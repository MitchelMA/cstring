#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "stringbuilder.h"
#include "str.h"

#define NULL_CHECK(STRINGPTR, RETURN)                                                     \
    do                                                                                    \
    {                                                                                     \
        if((STRINGPTR) == NULL || (STRINGPTR)->count_ == 0 || (STRINGPTR)->text_ == NULL) \
            return (RETURN);                                                              \
    } while(0)

string_t string_create(const char* c_str)
{
    size_t len = strlen(c_str);
    string_t str = {0};
    str.count_ = len;
    str.text_ = malloc(len);
    memcpy(str.text_, c_str, len);
    return str;
}

bool string_clean(string_t* string)
{
    NULL_CHECK(string, false);

    free(string->text_);
    string->text_ = NULL;
    string->count_ = 0;
    return true;
}

char* string_c_str(const string_t* string)
{
    NULL_CHECK(string, NULL);

    char* copy = malloc(string->count_ + 1);
    memcpy(copy, string->text_, string->count_);
    copy[string->count_] = '\0';
    return copy;
}

void string_output(FILE* fd, const string_t* string)
{
    // building the format string.
    stringbuilder_t format_builder = stringbuilder_create();
    stringbuilder_append_cstr(&format_builder, "%."); 
    stringbuilder_stoa(&format_builder, string->count_);
    stringbuilder_append_ch(&format_builder, 's');
    char* format_cstr = stringbuilder_build_cstr(&format_builder);
    stringbuilder_clean(&format_builder);

    // using the format-string to output to the file-descriptor.
    fprintf(fd, format_cstr, string->text_);

    free(format_cstr);
}

string_t string_itoa(int value)
{
    stringbuilder_t tmp = stringbuilder_create();
    stringbuilder_itoa(&tmp, value);
    string_t str = stringbuilder_build(&tmp);
    stringbuilder_clean(&tmp);
    return str;
}

string_t string_ltoa(long value)
{
    stringbuilder_t tmp = stringbuilder_create();
    stringbuilder_ltoa(&tmp, value);
    string_t str = stringbuilder_build(&tmp);
    stringbuilder_clean(&tmp);
    return str;
}

string_t string_lltoa(long long value)
{
    stringbuilder_t tmp = stringbuilder_create();
    stringbuilder_lltoa(&tmp, value);
    string_t str = stringbuilder_build(&tmp);
    stringbuilder_clean(&tmp);
    return str;
}

string_t string_stoa(size_t value)
{
    stringbuilder_t tmp = stringbuilder_create();
    stringbuilder_stoa(&tmp, value);
    string_t str = stringbuilder_build(&tmp);
    stringbuilder_clean(&tmp);
    return str;
}

int string_atoi(const string_t* string)
{
    NULL_CHECK(string, 0);

    int value = 0;
    int sign = 1;
    size_t idx = 0;
    char* chars = string->text_;

    if(*chars == '+' || *chars == '-')
    {
        if(*chars++ == '-') sign = -1;
        idx++;
    }

    while(idx < string->count_ && isdigit(*chars))
    {
        value *= 10;
        value += (int) (*chars++ - '0');
        idx++;
    }

    return value * sign;
}

long string_atol(const string_t* string)
{
    NULL_CHECK(string, 0L);

    long value = 0;
    int sign = 1;
    size_t idx = 0;
    char* chars = string->text_;

    if(*chars == '+' || *chars == '-')
    {
        if(*chars++ == '-') sign = -1;
        idx++;
    }

    while(idx < string->count_ && isdigit(*chars))
    {
        value *= 10;
        value += (long) (*chars++ - '0');
        idx++;
    }

    return value * sign;
}

long long string_atoll(const string_t* string)
{
    NULL_CHECK(string, 0LL);

    long long value = 0;
    int sign = 1;
    size_t idx = 0;
    char* chars = string->text_;

    if(*chars == '+' || *chars == '-')
    {
        if(*chars++ == '-') sign = -1;
        idx++;
    }

    while(idx < string->count_ && isdigit(*chars))
    {
        value *= 10;
        value += (long long) (*chars++ - '0');
        idx++;
    }

    return value * sign;
}

size_t string_atos(const string_t* string)
{
    size_t value = 0;
    size_t idx =   0;
    char* chars = string->text_;

    if(*chars == '+' || *chars == '-')
    {
        chars++;
        idx++;
    }

    while(idx < string->count_ && isdigit(*chars))
    {
        value *= 10;
        value += (size_t) (*chars++ - '0');
        idx++;
    }

    return value;
}

string_t string_add_(const string_t* str1, ...)
{
    va_list args;
    va_start(args, str1);
    string_t* current_string;
    stringbuilder_t builder = stringbuilder_create();

    stringbuilder_append_str(&builder, str1);
    while((current_string = va_arg(args, string_t*)) != NULL)
    {
        stringbuilder_append_str(&builder, current_string);
    }

    string_t ret = stringbuilder_build(&builder);
    stringbuilder_clean(&builder);
    va_end(args);

    return ret;
}