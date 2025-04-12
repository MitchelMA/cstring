#ifndef STRING_TYPES_H__
#define STRING_TYPES_H__

#include <vector.h>

typedef struct 
{
    size_t count_;
    char* text_;
} string_t;

typedef struct
{
    size_t start_idx;
    size_t count;
    const string_t* str_;
} stringview_t;

typedef struct
{
    vector_t* char_vector_;
} stringbuilder_t;

#endif // STRING_TYPES_H__
