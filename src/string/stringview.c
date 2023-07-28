#include <stdlib.h>
#include <string.h>

#include "stringview.h"
#include "stringbuilder.h"

#define NULL_CHECK(STR_V_PTR, RETURN)                                               \
do                                                                                  \
{                                                                                   \
    if((STR_V_PTR) == NULL || (STR_V_PTR)->str_ == NULL || (STR_V_PTR)->count == 0) \
        return (RETURN);                                                            \
} while(0)

#define NULL_CHECK_STR(STR_PTR, RETURN)                                         \
do                                                                              \
{                                                                               \
    if((STR_PTR) == NULL || (STR_PTR)->text_ == NULL || (STR_PTR)->count_ == 0) \
        return (RETURN);                                                        \
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

    char* copy = malloc(stringview->count + 1);
    memcpy(copy, stringview->str_->text_ + stringview->start_idx, stringview->count);
    copy[stringview->count] = '\0';
    return copy;
}

void stringview_output(FILE* fd, const stringview_t* stringview)
{
    stringbuilder_t format_builder = stringbuilder_create();
    stringbuilder_append_cstr(&format_builder, "%.");
    stringbuilder_stoa(&format_builder, stringview->count);
    stringbuilder_append_ch(&format_builder, 's');
    char* cstr_format = stringbuilder_build_cstr(&format_builder);
    stringbuilder_clean(&format_builder);
    fprintf(fd, cstr_format, (stringview->str_->text_ + stringview->start_idx));
    free(cstr_format);
}