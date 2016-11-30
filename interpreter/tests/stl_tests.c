#include <stdio.h>
#include <assert.h>
#include "../native/lni.h"
#include "stl.c"
#include <math.h>
#include <string.h>
#include <stdlib.h>

 /**
  * Unit tests (prints instead of asserts)
  */
 int main() {
    printf("Testing the stl library \n");

    lni_object* rand_test = random_number();
    printf("Random number: %d\n", rand_test->as_int);

    lni_object* number_1 = lni_new_int(1);
    lni_object* number_4 = lni_new_int(4);
    lni_object* number_6 = lni_new_int(6);

    lni_object* mod_test = modulo(number_6, number_4);
    printf("Modulo; number should be 2: %d\n", mod_test->as_int);

    lni_object* power_test = power(number_6, number_4);
    printf("Power; number should be 1296: %d\n", power_test->as_int);

    lni_object* number_neg_6 = lni_new_int(-6);
    lni_object* absolute_val_test = absolute_value(number_neg_6);
    printf("Absolute value; number should be 6: %d\n", absolute_val_test->as_int);

    lni_object** new_list_arr = malloc(sizeof(**new_list_arr) * 3);
    new_list_arr[0] = number_1;
    new_list_arr[1] = number_4;
    new_list_arr[2] = number_6;
    lni_object* list_one = lni_new_list(new_list_arr, 3);
    lni_object* list_two = reverse_list(list_one);
    lni_object* list_three = cat_list(list_one, list_two);

    printf("List length should be 6: %d\n", list_length(list_three)->as_int);
    printf("List should contain 1 4 6 6 4 1 (cat and reverse):");
    for(int i = 0; i < 6; i++) {
        printf(" %d", list_three->as_list[i]->as_int);
    }
    printf("\n");

    lni_object* list_four = push_front(list_three, lni_new_int(3));
    printf("Get front should be 3: %d\n", get_front(list_four)->as_int);

    lni_object* list_five = push_back(list_four, lni_new_int(5));
    printf("Get back should be 5: %d\n", get_back(list_five)->as_int);

    lni_object* list_six = set(list_five, lni_new_int(1), lni_new_int(10));
    printf("List length should be 8: %d\n", list_length(list_six)->as_int);
    printf("List should contain 3 10 4 6 6 4 1 5 (set):");
    for(int i = 0; i < 8; i++) {
        printf(" %d", list_six->as_list[i]->as_int);
    }
    printf("\n");

    char new_string_arr[10] = "hello";
    lni_object* string_one = lni_new_string(new_string_arr);
    printf("String length should be 5: %d\n", string_length(string_one)->as_int);

    lni_object* string_two = reverse_string(string_one);
    printf("Reverse string should be olleh: %s\n", string_two->as_string);

    lni_object* string_three = substring(string_two, lni_new_int(1), lni_new_int(4));
    printf("Substring should be lle: %s\n", string_three->as_string);

    char new_string_two_arr[10] = "123";
    lni_object* string_four = lni_new_string(new_string_two_arr);
    printf("Convert to int should be 123: %d\n", convert_to_int(string_four)->as_int);

    return 0;
 }