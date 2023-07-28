#include <stdio.h>
#include <stdlib.h>

#include "string/stringbuilder.h"
#include "string/stringview.h"

int main(void)
{
    stringbuilder_t my_str_bld = stringbuilder_create();
    stringbuilder_append_cstr(&my_str_bld, "-19Ade");
    string_t my_str = stringbuilder_build(&my_str_bld);
    stringview_t str_view = stringview_create(&my_str, 1, 2);
    stringview_print(&str_view);
    puts("");
    string_print(&my_str);
    puts("");

    int output = string_atoi(&my_str);
    printf("Output: %d\n", output);

    stringbuilder_clean(&my_str_bld);
    string_clean(&my_str);
    return EXIT_SUCCESS;
}