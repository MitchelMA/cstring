#include <stdio.h>
#include <stdlib.h>

#include "string/stringbuilder.h"

int main(void)
{
    stringbuilder_t my_str_bld = stringbuilder_create();
    stringbuilder_append_cstr(&my_str_bld, "-19Ade");
    string_t my_str = stringbuilder_build(&my_str_bld);
    string_print(&my_str);
    puts("");

    size_t output = string_atos(&my_str);
    printf("Output: %zu\n", output);

    stringbuilder_clean(&my_str_bld);
    string_clean(&my_str);
    return EXIT_SUCCESS;
}