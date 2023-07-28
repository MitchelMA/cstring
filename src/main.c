#include <stdio.h>
#include <stdlib.h>

#include "string/stringbuilder.h"

int main(void)
{
    string_t my_str = string_create("Hello, World!\n");
    string_t my_str2 = string_stoa(__SIZE_MAX__);
    string_t my_str3 = string_create("\nGoedendag!\n");
    string_t my_str4 = string_add(&my_str, &my_str2, &my_str3);
    string_print(&my_str4);

    string_clean(&my_str);
    string_clean(&my_str2);
    string_clean(&my_str3);
    string_clean(&my_str4);
    return EXIT_SUCCESS;
}