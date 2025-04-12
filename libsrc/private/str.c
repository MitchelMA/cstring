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

size_t get_first_non_occurence_(const string_t* string, const char* delim);
size_t get_last_non_occurence_(const string_t* string, const char* delim);

static int qsort_char_compare_(const void* a, const void* b);
static int qsort_char_compare_inverse_(const void* a, const void* b);


string_t string_create(const char* c_str)
{
    size_t len = strlen(c_str);
    size_t byte_count = sizeof(char) * len;
    string_t str = {0};

    str.count_ = len;
    str.text_ = malloc(byte_count);
    if(str.text_ == NULL)
        return string_empty;

    memcpy(str.text_, c_str, byte_count);
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

size_t string_length(const string_t* string)
{
    NULL_CHECK(string, 0);
    return string->count_;
}

string_t string_copy(const string_t* src)
{
    NULL_CHECK(src, string_empty);
    size_t byte_count = sizeof(char) * src->count_;

    char* copy = malloc(byte_count);
    if(copy == NULL)
        return string_empty;

    memcpy(copy, src->text_, byte_count);
    return (string_t){src->count_, copy};
}

char* string_cstr(const string_t* string)
{
    NULL_CHECK(string, NULL);
    size_t byte_count = sizeof(char) * string->count_;

    char* copy = malloc(byte_count + sizeof(char));
    if(copy == NULL)
        return NULL;

    memcpy(copy, string->text_, byte_count);
    copy[string->count_] = '\0';
    return copy;
}

bool string_output(FILE* fstream, const string_t* string)
{
    NULL_CHECK(string, false);

    size_t count = string->count_;
    char* data = string->text_;

    fprintf(fstream, "%.*s", (int)count, data);
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

    if(file_size == 0)
        return string_empty;

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

    return strncmp(
        str1->text_,
        str2->text_,
        len
    ) == 0;
}

vector_t* string_split(const string_t* string, const char* delim)
{
    NULL_CHECK(string, NULL);

    // removing trailing en prepended delim-strings
    // calculate the offsets from the strings with trailings removed
    size_t min_start = get_first_non_occurence_(string, delim);
    size_t max_count = get_last_non_occurence_(string, delim);
    max_count = max_count > 0 ? max_count + 1 : string->count_;

    stringview_t current_view = {min_start, 0, string};
    vector_t* view_vec = vector_create(sizeof(stringview_t));
    stringbuilder_t delim_buffer = stringbuilder_create();
    size_t delim_len = strlen(delim);

    while((current_view.start_idx + current_view.count) < (min_start + max_count))
    {
        bool found_full_delim = false;
        char current_char = current_view.str_->text_[current_view.start_idx + current_view.count];
        if(strchr(delim, current_char))
        {
            stringbuilder_append_ch(&delim_buffer, current_char);
            if (stringbuilder_compare_cstr(&delim_buffer, delim))
            {
                stringbuilder_reset(&delim_buffer);
                found_full_delim = true;
            }
        }
        else
        {
            stringbuilder_reset(&delim_buffer);
        }

        if(found_full_delim)
        {
            current_view.count -= (delim_len - 1);
            vector_append(view_vec, (void*) &current_view);
            current_view.start_idx += current_view.count + delim_len;
            current_view.count = 0;

            continue;
        }

        current_view.count++;
    }

    if(current_view.count > 0)
    {
        vector_append(view_vec, (void*) &current_view);
    }
    stringbuilder_clean(&delim_buffer);
    return view_vec;
}

void string_sort_alpha(string_t* string)
{
    if (string == NULL || string->text_ == NULL || string->count_ == 0)
      return;

    qsort(string->text_, string->count_, sizeof(char), qsort_char_compare_);
}

void string_sort_alpha_inverse(string_t* string)
{
    if (string == NULL || string->text_ == NULL || string->count_ == 0)
      return;

    qsort(string->text_, string->count_, sizeof(char), qsort_char_compare_inverse_);
}

string_t string_remove_from_start(const string_t* string, const char* remove)
{
    NULL_CHECK(string, string_empty);
    
    size_t first_non = get_first_non_occurence_(string, remove);
    stringview_t view = stringview_create(string, first_non, 0);
    return stringview_owning(&view);
}

string_t string_remove_from_end(const string_t* string, const char* remove)
{
    NULL_CHECK(string, string_empty);

    size_t last_non = get_last_non_occurence_(string, remove);
    if (last_non == 0)
        return string_copy(string);
    stringview_t view = stringview_create(string, 0, last_non+1);
    return stringview_owning(&view);
}

string_t string_remove_start_end(const string_t* string, const char* remove)
{
    NULL_CHECK(string, string_empty);

    stringview_t view = {0};
    size_t first_non = get_first_non_occurence_(string, remove);
    size_t last_non  = get_last_non_occurence_(string, remove);

    if(last_non == 0)
    {
        view = stringview_create(string, first_non, 0);
    }
    else
    {
        view = stringview_create(string, first_non, last_non - first_non + 1);
    }

    return stringview_owning(&view);
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

size_t get_first_non_occurence_(const string_t* string, const char* delim)
{
    NULL_CHECK(string, 0);

    stringbuilder_t delim_buffer = stringbuilder_create();
    stringview_t view = {0, 0, string};
    size_t delim_len = strlen(delim);
    size_t idx = 0;
    
    while((view.start_idx + view.count) < view.str_->count_)
    {
        char current_char = view.str_->text_[view.start_idx + view.count];
        if(!strchr(delim, current_char))
            break;
        
        stringbuilder_append_ch(&delim_buffer, current_char);
        if(vector_get_elem_count(delim_buffer.char_vector_) > delim_len)
            stringbuilder_reset(&delim_buffer);

        if (stringbuilder_compare_cstr(&delim_buffer, delim))
        {
            stringbuilder_reset(&delim_buffer);
            idx = view.start_idx + view.count + 1;
            view.start_idx += view.count + 1;
            view.count = 0;
            continue;
        }

        view.count++;
    }

    stringbuilder_clean(&delim_buffer);

    return idx;
}

size_t get_last_non_occurence_(const string_t* string, const char* delim)
{
    NULL_CHECK(string, 0);

    stringbuilder_t delim_buffer = stringbuilder_create();
    stringview_t view = {string->count_-1, 0, string};
    size_t delim_len = strlen(delim);
    size_t idx = 0;

    while((view.start_idx + view.count) > 0)
    {
        char current_char = view.str_->text_[view.start_idx + view.count];
        if(!strchr(delim, current_char))
            break;

        stringbuilder_push_ch(&delim_buffer, current_char);
        if(vector_get_elem_count(delim_buffer.char_vector_) > delim_len)
            stringbuilder_reset(&delim_buffer);

        if (stringbuilder_compare_cstr(&delim_buffer, delim))
        {
            stringbuilder_reset(&delim_buffer);
            idx = view.start_idx + view.count - 1;
            view.start_idx -= 1;
            view.count = 0;
            continue;
        }

        view.start_idx--;
    }

    stringbuilder_clean(&delim_buffer);
    return idx;
}

int qsort_char_compare_(const void* a, const void* b)
{
    char* a_char = (char*)a;
    char* b_char = (char*)b;

    return *a_char - *b_char;
}

int qsort_char_compare_inverse_(const void* a, const void* b)
{
    char* a_char = (char*)a;
    char* b_char = (char*)b;

    return *b_char - *a_char;
}
