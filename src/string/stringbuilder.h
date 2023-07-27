#ifndef STRINGBUILDER_H__
#define STRINGBUILDER_H__

#include <vector.h>

typedef struct
{
    vector_t char_vector_;
} stringbuilder_t;

[[nodiscard]]
stringbuilder_t stringbuilder_create(void);
bool stringbuilder_clean(stringbuilder_t* str_builder);

bool stringbuilder_append_ch(stringbuilder_t* str_builder, char character);
bool stringbuilder_append_cstr(stringbuilder_t* str_builder, const char* cstr);
[[nodiscard]]
char* stringbuilder_as_cstr(const stringbuilder_t* str_builder);

#endif // STRINGBUILDER_H__