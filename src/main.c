#include <stdio.h>
#include <stdlib.h>

#include "string/stringbuilder.h"
#include "string/stringview.h"

int main(void)
{
    stringbuilder_t my_str_bld = stringbuilder_create();
    stringbuilder_append_cstr(&my_str_bld, "-19Ade");
    string_t my_str = stringbuilder_build(&my_str_bld);
    stringview_t str_view = stringview_create(&my_str, 1, 3);
    printf("String: ");
    string_print(&my_str);
    printf("\nString-view: ");
    stringview_print(&str_view);
    puts("");

    int output = string_atoi(&my_str);
    printf("Output: %d\n", output);
    output = stringview_atoi(&str_view);
    printf("Output from string-view: %d\n", output);

    stringbuilder_read_console(&my_str_bld);
    stringbuilder_read_console(&my_str_bld);
    string_t input_str = stringbuilder_build(&my_str_bld);
    string_print(&input_str);

    stringbuilder_clean(&my_str_bld);
    string_clean(&my_str);
    string_clean(&input_str);
    return EXIT_SUCCESS;
}