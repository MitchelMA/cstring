#include <stdio.h>
#include <stdlib.h>

#include "string/stringbuilder.h"

int main(void)
{
    stringbuilder_t str_builder = stringbuilder_create();

    stringbuilder_append_cstr(&str_builder, "Hello, World!\n");
    stringbuilder_append_cstr(&str_builder, "\tIs dit niet cool?");

    char* str = stringbuilder_as_cstr(&str_builder);
    stringbuilder_append_ch(&str_builder, '!');
    char* str_2 = stringbuilder_as_cstr(&str_builder);

    printf("Built string: %s\n", str);
    printf("\n%s\n", str_2);

    stringbuilder_clean(&str_builder);
    free(str);
    free(str_2);

    return EXIT_SUCCESS;
}