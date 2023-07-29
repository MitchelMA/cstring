#include <stdio.h>
#include <stdlib.h>

#include "string/stringbuilder.h"
#include "string/stringview.h"

int main(void)
{
    stringbuilder_t str_builder = stringbuilder_create();
    string_t str1 = string_create("Goeie!");
    string_t str2 = string_create("Zeker ");
    stringview_t strv1 = stringview_create(&str1, 1, 3);
    stringview_t strv2 = stringview_create(&str2, 2, 1);
    stringbuilder_append_strv(&str_builder, &strv1);
    stringbuilder_push_str(&str_builder, &str2);
    stringbuilder_insert_strv(&str_builder, 2, &strv2);
    size_t idx = 4;
    char* character = stringbuilder_char_at(&str_builder, idx);
    *character = stringbuilder_pop(&str_builder);
    *character = stringbuilder_dequeue(&str_builder);
    string_t str3 = stringbuilder_build(&str_builder);

    string_print(&str3);
    puts("");
    
    printf("char at index %zu: %c\n", idx, *character);
    string_t str4 = string_add(&str2, &str1);
    string_print(&str4);
    puts("");


    string_clean(&str1);
    string_clean(&str2);
    string_clean(&str3);
    string_clean(&str4);

    stringbuilder_reset(&str_builder);
    printf("Input: ");
    stringbuilder_read_console(&str_builder);
    // removing the 'enter' input we took from the console
    char dequeued = stringbuilder_dequeue(&str_builder);
    if(dequeued != '\n')
    {
        stringbuilder_append_ch(&str_builder, dequeued);
        puts("");
    }

    str1 = stringbuilder_build(&str_builder);
    printf("Read:  ");
    string_print(&str1);
    puts("");

    string_clean(&str1);
    stringbuilder_clean(&str_builder);
    return EXIT_SUCCESS;
}