#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "stringbuilder.h"
#include "str.h"
#include "stringview.h"

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

int64_t get_first_non_occurrence_(const string_t* string, const char* match);
int64_t get_last_non_occurrence_(const string_t* string, const char* match);

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

bool string_is_palindrome(const string_t* string)
{
    NULL_CHECK(string, false);

    size_t check_length = string->count_ / 2 + string->count_ % 2;
    for (int64_t check_i = 0; check_i < (int64_t)check_length; ++check_i)
    {
        int64_t index_from_end = ((int64_t)string->count_ - 1) - check_i;
        if (string->text_[check_i] != string->text_[index_from_end])
            return false;
    }

    return true;
}

int64_t string_find_cstr(const string_t* string, const char* match, int occurrence_index)
{
    NULL_CHECK(string, -1);
    
    if (match == NULL)
        return -1;

    size_t match_length = strlen(match);

    if (match_length == 0 || match_length >= string->count_)
        return -1;

    stringview_t window = stringview_create(string, 0, match_length);
    size_t endIdx = string->count_ - (match_length - 1);

    size_t letter_index = 0;
    while (letter_index < endIdx)
    {
        window.start_idx = letter_index;
        bool is_found = stringview_compare_cstr(&window, match);

        if (is_found)
        {
            if (occurrence_index <= 0)
                return letter_index;

            occurrence_index--;
            letter_index += match_length;

            continue;
        }

        letter_index++;
    }

    return -1;
}

vector_t* string_split(const string_t* string, const char* delim)
{
    NULL_CHECK(string, NULL);

    if (delim == NULL)
        return NULL;

    size_t delim_length = strlen(delim);

    if (delim_length == 0 || delim_length >= string->count_)
        return NULL;

    vector_t* view_vector = vector_create(sizeof(stringview_t));
    stringview_t window = stringview_create(string, 0, delim_length);
    size_t anchor = 0;
    size_t endIdx = string->count_ - (window.count - 1);

    size_t index = 0;
    while (index < endIdx)
    {
        window.start_idx = index;
        bool is_found = stringview_compare_cstr(&window, delim);
        
        if (is_found)
        {
            stringview_t anchor_view = stringview_create(string, anchor, index - anchor);

            if (index > anchor)
                vector_append(view_vector, (void*)&anchor_view);

            index += delim_length;
            anchor = index;
            continue;
        }

        index++;
    }

    stringview_t remaining_view = stringview_create(string, anchor, string->count_ - anchor);
    if (remaining_view.count > 0)
        vector_append(view_vector, (void*)&remaining_view);

    return view_vector;
}

size_t string_split_static(const string_t* string, const char* delim, stringview_t* output, size_t output_size)
{
    NULL_CHECK(string, 0);

    if (delim == NULL || output == NULL || output_size == 0)
        return 0;

    size_t delim_length = strlen(delim);

    if (delim_length == 0 || delim_length >= string->count_)
        return 0;

    stringview_t window = stringview_create(string, 0, delim_length);
    size_t anchor_index = 0;
    size_t end_index = string->count_ - (window.count - 1);
    size_t processed = 0;
    size_t index = 0;

    while (index < end_index && processed < output_size)
    {
        window.start_idx = index;
        bool is_found = stringview_compare_cstr(&window, delim);

        if (is_found)
        {
            stringview_t anchor_view = stringview_create(string, anchor_index, index - anchor_index);

            if (index > anchor_index)
            {
                output[processed] = anchor_view;
                ++processed;
            }

            index += delim_length;
            anchor_index = index;
            continue;
        }

        ++index;
    }

    stringview_t remaining_view = stringview_create(string, anchor_index, string->count_ - anchor_index);
    if (remaining_view.count > 0 && processed < output_size)
        output[processed++] = remaining_view;

    return processed;
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

string_t string_remove_match(const string_t* string, const char* match)
{
    NULL_CHECK(string, string_empty);

    vector_t* view_vector = string_split(string, match);

    if (view_vector == NULL)
        return string_empty;

    stringbuilder_t end_builder = stringbuilder_create();
    size_t vector_size = vector_get_elem_count(view_vector);
    stringview_t* start_addr = (stringview_t*)vector_get_start_addr_(view_vector);

    for (size_t i = 0; i < vector_size; ++i)
        stringbuilder_append_strv(&end_builder, (start_addr + i));

    vector_clean(view_vector);
    string_t end_string = stringbuilder_build(&end_builder);
    stringbuilder_clean(&end_builder);

    return end_string;
}

string_t string_replace_match(const string_t* string, const char* match, const char* replace)
{
    NULL_CHECK(string, string_empty);

    if (match == NULL || replace == NULL)
        return string_empty;

    size_t match_length = strlen(match);
    size_t replacement_length = strlen(replace);

    if (match_length == 0 || replacement_length == 0 || match_length >= string->count_)
        return string_empty;

    stringbuilder_t builder = stringbuilder_create();

    stringview_t window = stringview_create(string, 0, match_length);
    size_t end_idx = string->count_ - (match_length - 1);

    size_t letter_index = 0;
    while (letter_index < end_idx)
    {
        window.start_idx = letter_index;
        bool is_found = stringview_compare_cstr(&window, match);

        if (is_found)
        {
            stringbuilder_append_cstr(&builder, replace);
            letter_index += match_length;
            continue;
        }

        stringbuilder_append_ch(&builder, string->text_[letter_index]);
        letter_index++;
    }

    stringview_t remaining_view = stringview_create(string, letter_index, string->count_ - letter_index);
    if (remaining_view.count > 0)
        stringbuilder_append_strv(&builder, &remaining_view);

    string_t end_string = stringbuilder_build(&builder);
    stringbuilder_clean(&builder);

    return end_string;
}

stringview_t string_trim_start(const string_t* string, const char* remove)
{
    NULL_CHECK(string, (stringview_t){0});
    
    int64_t first_non = get_first_non_occurrence_(string, remove);
    return stringview_create(string, first_non, 0);
}

stringview_t string_trim_end(const string_t* string, const char* remove)
{
    NULL_CHECK(string, (stringview_t){0});

    int64_t last_non = get_last_non_occurrence_(string, remove);
    if (last_non == -1)
        return (stringview_t){0};

    return stringview_create(string, 0, last_non+1);
}

stringview_t string_trim_both(const string_t* string, const char* remove)
{
    NULL_CHECK(string, (stringview_t){0});

    stringview_t view = {0};
    int64_t first_non = get_first_non_occurrence_(string, remove);
    int64_t last_non  = get_last_non_occurrence_(string, remove);

    if(last_non == -1)
    {
        view = stringview_create(string, first_non, 0);
    }
    else
    {
        view = stringview_create(string, first_non, last_non - first_non + 1);
    }

    return view; 
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

int64_t get_first_non_occurrence_(const string_t* string, const char* match)
{
    NULL_CHECK(string, -1);

    if (match == NULL)
        return -1;

    size_t match_length = strlen(match);
    if (match_length == 0 || match_length >= string->count_)
        return -1;

    stringview_t window = stringview_create(string, 0, match_length);
    size_t endIdx = string->count_ - (match_length - 1);

    size_t index = 0;
    while (index < endIdx)
    {
        window.start_idx = index;

        bool is_found = stringview_compare_cstr(&window, match);
        if (!is_found)
            return index;
        
        index += match_length;
    }

    return -1;
}

int64_t get_last_non_occurrence_(const string_t* string, const char* match)
{
    NULL_CHECK(string, -1);

    if (match == NULL)
        return -1;

    size_t match_length = strlen(match);
    if (match_length == 0 || match_length >= string->count_)
        return -1;

    stringview_t window = stringview_create(string, string->count_ - match_length, match_length);

    int64_t index = window.start_idx;
    while (index > 0)
    {
        window.start_idx = index;

        bool is_found = stringview_compare_cstr(&window, match);
        if (!is_found)
          return index;

        index -= match_length;
    }

    return -1;
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
