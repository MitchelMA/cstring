#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    #include <conio.h>
    #include <windows.h>
#elif defined(__linux__)
    #include <fcntl.h>
#endif // os-check

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

stringbuilder_t stringbuilder_create_from_cstr(const char* cstr)
{
    stringbuilder_t builder = {0};
    builder.char_vector_ = vector_create_from(sizeof(char), strlen(cstr), (void*) cstr);
    return builder;
}

stringbuilder_t stringbuilder_create_from_str(const string_t* str)
{
    stringbuilder_t builder = {0};
    builder.char_vector_ = vector_create_from(sizeof(char), str->count_, (void*) str->text_);
    return builder;
}

stringbuilder_t stringbuilder_create_from_strv(const stringview_t* strv)
{
    stringbuilder_t builder = {0};
    builder.char_vector_ = vector_create_from(sizeof(char), strv->count, (void*) (strv->str_->text_ + strv->start_idx));
    return builder;
}

bool stringbuilder_clean(stringbuilder_t* str_builder)
{
    NULL_CHECK(str_builder, false);

    vector_clean(str_builder->char_vector_);
    str_builder->char_vector_ = NULL;
    return true;
}

bool stringbuilder_reset(stringbuilder_t* str_builder)
{
    NULL_CHECK(str_builder, false);
    vector_clean(str_builder->char_vector_);
    str_builder->char_vector_ = vector_create(sizeof(char));
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

bool stringbuilder_append_strv(stringbuilder_t* str_builder, const stringview_t* strv)
{
    NULL_CHECK(str_builder, false);
    for(size_t i = 0; i < strv->count; i++)
        vector_append(str_builder->char_vector_, 
                     (void*) (strv->str_->text_ + strv->start_idx + i));
    
    return true;
}

bool stringbuilder_push_ch(stringbuilder_t* str_builder, char character)
{
    NULL_CHECK(str_builder, false);

    vector_push(str_builder->char_vector_, (void*) &character);
    return true;
}

bool stringbuilder_push_cstr(stringbuilder_t* str_builder, const char* cstr)
{
    NULL_CHECK(str_builder, false);

    vector_t tmp = vector_create_from(sizeof(char), strlen(cstr), (void*) cstr);
    bool pushed = vector_push_to(str_builder->char_vector_, tmp, 0, 0);
    vector_clean(tmp);
    return pushed;
}

bool stringbuilder_push_str(stringbuilder_t* str_builder, const string_t* str)
{
    NULL_CHECK(str_builder, false);
    
    vector_t tmp = vector_create_from(sizeof(char), str->count_, (void*) str->text_);
    bool pushed = vector_push_to(str_builder->char_vector_, tmp, 0, 0);
    vector_clean(tmp);
    return pushed;
}

bool stringbuilder_push_strv(stringbuilder_t* str_builder, const stringview_t* strv)
{
    NULL_CHECK(str_builder, false);

    vector_t tmp = vector_create_from(sizeof(char), strv->count, (void*) (strv->str_->text_ + strv->start_idx));
    bool pushed = vector_push_to(str_builder->char_vector_, tmp, 0, 0);
    vector_clean(tmp);
    return pushed;
}

bool stringbuilder_insert_ch(stringbuilder_t* str_builder, size_t idx, char character)
{
    NULL_CHECK(str_builder, false);
    return vector_insert_to(str_builder->char_vector_, idx, (void*) &character);
}

bool stringbuilder_insert_cstr(stringbuilder_t* str_builder, size_t idx, const char* cstr)
{
    NULL_CHECK(str_builder, false);

    vector_t tmp = vector_create_from(sizeof(char), strlen(cstr), (void*) cstr);
    bool copied = vector_copy_into(str_builder->char_vector_, idx, tmp, 0, 0);
    vector_clean(tmp);
    return copied;
}

bool stringbuilder_insert_str(stringbuilder_t* str_builder, size_t idx, const string_t* str)
{
    NULL_CHECK(str_builder, false);
    vector_t tmp = vector_create_from(sizeof(char), str->count_, (void*) str->text_);
    bool copied = vector_copy_into(str_builder->char_vector_, idx, tmp, 0, 0);
    vector_clean(tmp);
    return copied;
}

bool stringbuilder_insert_strv(stringbuilder_t* str_builder, size_t idx, const stringview_t* strv)
{
    NULL_CHECK(str_builder, false);
    vector_t tmp = vector_create_from(sizeof(char), strv->count, (void*) (strv->str_->text_ + strv->start_idx));
    bool copied = vector_copy_into(str_builder->char_vector_, idx, tmp, 0, 0);
    vector_clean(tmp);
    return copied;
}

char* stringbuilder_char_at(const stringbuilder_t* str_builder, size_t idx)
{
    return ((char*) vector_get_start_addr_(str_builder->char_vector_) + idx);
}

char stringbuilder_pop(stringbuilder_t* str_builder)
{
    char popped = 0;
    NULL_CHECK(str_builder, popped);

    vector_pop(str_builder->char_vector_, &popped);
    return popped;
}

char stringbuilder_dequeue(stringbuilder_t* str_builder)
{
    char dequeued = 0;
    NULL_CHECK(str_builder, dequeued);

    vector_dequeue(str_builder->char_vector_, &dequeued);
    return dequeued;
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

bool stringbuilder_read(FILE* fstream, stringbuilder_t* str_builder)
{
    NULL_CHECK(str_builder, false);
    fseek(fstream, 0, SEEK_SET);

    int ch = fgetc(fstream);
    if(ch == EOF || ch == '\n')
        return false;
    ungetc(ch, fstream);

    int filenumber = fileno(fstream);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)

    // TODO!! TEST THIS ON A WINDOWS MACHINE!

    HANDLE hf = _get_osfhandle(_fileno(fstream));
    DWORD bytes_evail = 0;
    PeekNamedPipe(hf, NULL, 0, NULL, &bytes_evail, NULL);
    while(bytes_evail > 0)
    {
        ch = fgetc(fstream);
        vector_append(str_builder->char_vector_, (void*) &ch);
        PeekNamedPipe(hf, NULL, 0, NULL, &bytes_evail, NULL);
    }

#elif defined(__linux__)

    int def_flag = fcntl(filenumber, F_GETFL, O_ACCMODE);

    fcntl(filenumber, F_SETFL, O_NONBLOCK);
    while((ch = fgetc(fstream)) != EOF)
        vector_append(str_builder->char_vector_, (void*) &ch);
    fcntl(filenumber, F_SETFL, def_flag);

#endif // os-check

    return true;
}