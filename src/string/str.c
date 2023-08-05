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

#define ATON(STRINGPTR, TYPE, VALUE, SIGN, IDX, CHARS)      \
do                                                          \
{                                                           \
    if(*(CHARS) == '+' || *(CHARS) == '-')                  \
    {                                                       \
        if(*(CHARS)++ == '-') (SIGN) = -1;                  \
        (IDX)++;                                            \
    }                                                       \
                                                            \
    while((IDX) < (STRINGPTR)->count_ && isdigit(*(CHARS))) \
    {                                                       \
        (VALUE) *= 10;                                      \
        (VALUE) += (TYPE) (*(CHARS)++ - '0');               \
        (IDX)++;                                            \
    }                                                       \
} while(0)

string_t string_empty = {0, NULL};

//Todo! Be more pedantic with `sizeof(char)` operator. Currently not used consistently!

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

string_t string_copy(const string_t* src)
{
    NULL_CHECK(src, string_empty);

    char* copy = malloc(sizeof(char) * src->count_);
    if(copy == NULL)
        return string_empty;

    memcpy(copy, src->text_, sizeof(char) * src->count_);
    return (string_t){src->count_, copy};
}

char* string_c_str(const string_t* string)
{
    NULL_CHECK(string, NULL);

    char* copy = malloc(string->count_ + 1);
    memcpy(copy, string->text_, string->count_);
    copy[string->count_] = '\0';
    return copy;
}

bool string_output(FILE* fstream, const string_t* string)
{
    NULL_CHECK(string, false);

    // building the format string.
    stringbuilder_t format_builder = stringbuilder_create();
    stringbuilder_append_cstr(&format_builder, "%."); 
    stringbuilder_stoa(&format_builder, string->count_);
    stringbuilder_append_ch(&format_builder, 's');
    char* format_cstr = stringbuilder_build_cstr(&format_builder);
    stringbuilder_clean(&format_builder);

    // using the format-string to output to the file-descriptor.
    fprintf(fstream, format_cstr, string->text_);
    free(format_cstr);

    return true;
}

string_t string_read(FILE* fstream, size_t max_len)
{
    size_t file_size;
    char* content;

    if(fseek(fstream, 0, SEEK_END))
        return string_empty;

    long success = ftell(fstream);
    if(success == -1)
        return string_empty;

    file_size = (size_t) success;
    file_size = file_size > max_len ? max_len : file_size;
    fseek(fstream, 0, SEEK_SET);

    if((content = malloc(sizeof(char) * file_size)) == NULL)
        return string_empty;

    if(fread(content, sizeof(char), file_size/sizeof(char), fstream) != file_size && ferror(fstream))
    {
        free(content);
        return string_empty;
    }

    return (string_t){file_size, content};
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

    ATON(string, int, value, sign, idx, chars);

    return value * sign;
}

long string_atol(const string_t* string)
{
    NULL_CHECK(string, 0L);

    long value = 0;
    int sign = 1;
    size_t idx = 0;
    char* chars = string->text_;

    ATON(string, long, value, sign, idx, chars);

    return value * sign;
}

long long string_atoll(const string_t* string)
{
    NULL_CHECK(string, 0LL);

    long long value = 0;
    int sign = 1;
    size_t idx = 0;
    char* chars = string->text_;

    ATON(string, long long, value, sign, idx, chars);

    return value * sign;
}

size_t string_atos(const string_t* string)
{
    size_t value = 0;
    int sign = 0;
    size_t idx =   0;
    char* chars = string->text_;

    ATON(string, size_t, value, sign, idx, chars);
    UNUSED(sign);
    return value;
}

bool string_compare(const string_t* str1, const string_t* str2)
{
    if(str1 == NULL || str2 == NULL) return false;
    if(str1->count_ == 0 && str2->count_ == 0 &&
       str1->text_ == NULL && str2->text_ == NULL) return true;

    if(str1->count_ != str2->count_)
        return false;

    size_t len = str1->count_;

    for(size_t i = 0; i < len; i++)
    {
        if(str1->text_[i] != str2->text_[i])
            return false;
    }

    return true;
}

string_t string_add_(const string_t* str1, ...)
{
    va_list args;
    va_start(args, str1);
    string_t* current_string;
    stringbuilder_t builder = stringbuilder_create_from_str(str1);

    while((current_string = va_arg(args, string_t*)) != NULL)
    {
        stringbuilder_append_str(&builder, current_string);
    }

    string_t ret = stringbuilder_build(&builder);
    stringbuilder_clean(&builder);
    va_end(args);

    return ret;
}