#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "string/stringbuilder.h"
#include "string/stringview.h"

void num_example(void);
/**
 * Returns a vector containing `long long`'s
 * @param text
 * @return
 */
NODISCARD
vector_t* text_retrieve_numbers(const string_t* text);

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
    puts("");
    num_example();
    return EXIT_SUCCESS;
}

void num_example(void)
{
    stringbuilder_t builder;
    string_t text;
    vector_t* numbers = NULL;

    builder  = stringbuilder_create();
    printf("Text with numbers throughout: ");
    stringbuilder_read_console(&builder);
    char removed = stringbuilder_dequeue(&builder);
    if(removed != '\n') stringbuilder_append_ch(&builder, removed);

    text = stringbuilder_build(&builder);
    stringbuilder_clean(&builder);

    numbers = text_retrieve_numbers(&text);
    string_clean(&text);
    puts("Numbers found: ");
    for(size_t i = 0; i < vector_get_elem_count(numbers); i++)
    {
        long long* number;
        vector_elem_at(numbers, i, (void**) &number);
        printf("\t%lld\n", *number);
    }

    vector_clean(numbers);
}

vector_t* text_retrieve_numbers(const string_t* text)
{
    bool found_number = false;
    stringview_t str_view = stringview_create(text, 0, 0);
    str_view.count = 0;
    vector_t* num_vector = vector_create(sizeof(long long));
    long long number = 0;

    while((str_view.start_idx + str_view.count) < str_view.str_->count_)
    {
        char at_current_pos = str_view.str_->text_[str_view.start_idx + str_view.count];
        bool prev_value = found_number;

        found_number = ((at_current_pos == '-' || at_current_pos == '+') && str_view.count == 0) || isdigit(at_current_pos);
        if(prev_value == true && found_number == false)
        {
            number = stringview_atoll(&str_view);
            vector_append(num_vector, (void*) &number);
        }

        if(found_number)
        {
            str_view.count++;
        }
        else
        {
            str_view.start_idx += str_view.count == 0 ? 1 : str_view.count+1;
            str_view.count = 0;
        }
    }

    if(str_view.count > 0)
    {
        number = stringview_atoll(&str_view);
        vector_append(num_vector, (void*) &number);
    }

    return num_vector;
}