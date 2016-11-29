#include <stdio.h>
#include <assert.h>
#include <lni.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/**
 * Standard library written in C for L programs
 * Takes in L objects and returns L objects to interpreter
 */

 /**
 * Takes in a L objects and returns same L object to interpreter (for testing purposes)
 * @param input The object passed in
 * @return The same object passed in
 */
 lni_object* identity(lni_object* input) {
    return input;
 }

/**
 * Adds two numbers together (for testing purposes)
 * @param num_one First number to be added
 * @param num_two Second number to be added 
 * @return L object with value of sum of the two numbers
 */
 lni_object* add(lni_object* num_one, lni_object* num_two) {
    assert(num_one->type == LNI_INT);
    assert(num_two->type == LNI_INT);
    return lni_new_int(num_one->as_int + num_two->as_int);
 }

/**
 * Finds the modulo of two numbers
 * @param num_one First number to be moduloed (dividend) 
 * @param num_two Second number to be moduloed (divisor) 
 * @return L object with value of num_one % num_two
 */
 lni_object* modulo(lni_object* num_one, lni_object* num_two) {
    assert(num_one->type == LNI_INT);
    assert(num_two->type == LNI_INT);
    return lni_new_int(num_one->as_int % num_two->as_int);
 }

/**
 * Finds the power of two numbers
 * @param num_one First number in the exponent (base) 
 * @param num_two Second number in the exponent (power) 
 * @return L object with value of num_one^num_two
 */
 lni_object* pow(lni_object* num_one, lni_object* num_two) {
    assert(num_one->type == LNI_INT);
    assert(num_two->type == LNI_INT);
    return lni_new_int((int) pow((double) num_one->as_int, (double) num_two->as_int);
 }

/**
 * Finds the absolute value of a number
 * @param num Number to find absolute value of
 * @return L object with value of absolute value of num
 */
 lni_object* abs(lni_object* num) {
    assert(num->type == LNI_INT);
    int primitive_num = num->as_int;
    if(primitive_num < 0) {
        primitive_num = -primitive_num;
    }
    return lni_new_int(primitive_num);
 }

/**
 * Returns the object in the list at the index specified
 * @param list The list
 * @param index Index inside the list
 * @return L object at the specified index
 */
 lni_object* get(lni_object* list, lni_object* index) {
    assert(list->type == LNI_LIST);
    assert(index->type == LNI_INT);
    assert(list->as_list_len < index->as_int && index->as_int >= 0);
    return (lni_object*) list->as_list[index];
 }

/**
 * Sets the object in the list at the index specified
 * @param list The list
 * @param index Index inside the list
 * @param set_object The object which to set
 * @return The list
 */
 lni_object* set(lni_object* list, lni_object* index, lni_object* set_object) {
    assert(list->type == LNI_LIST);
    assert(index->type == LNI_INT);
    assert(list->as_list_len < index->as_int && index->as_int >= 0);
    //free(list->as_list[index]); //optional
    list->as_list[index] = set_object;
    return list;
 }
  
/**
 * Gets the first object in the list
 * @param list The list
 * @return The object at index 0 of the list (or null if the list is empty)
 */
 lni_object* get_front(lni_object* list) {
    assert(list->type == LNI_LIST);
    if(list->as_list_len == 0) {
        return lni_new_nil();
    } else {
        return list->as_list[0];
    }
 }

/**
 * Gets the last object in the list
 * @param list The list
 * @return The object at end of the list (or null if the list is empty)
 */
 lni_object* get_back(lni_object* list) {
    assert(list->type == LNI_LIST);
    if(list->as_list_len == 0) {
        return lni_new_nil();
    } else {
        return list->as_list[list->as_list_len - 1];
    }
 }

/**
 * Gets the length of the list (only the first layer)
 * @param list The list
 * @return Length of the list
 */
 lni_object* list_length(lni_object* list) {
    assert(list->type == LNI_LIST);
    return lni_new_int(list->as_list_len);
 }

/**
 * Joins two lists together
 * @param first_list The first part of the concatenated list
 * @param second_list The second part of the concatenated list
 * @return The concatenated list
 */
 lni_object* cat_list(lni_object* first_list, lni_object* second_list) {
    assert(first_list->type == LNI_LIST);
    assert(second_list->type == LNI_LIST);
    lni_object** new_list_arr = malloc(sizeof(**new_list_arr) * (first_list->as_list_len + second_list->as_list_len));
    for(int i = 0; i < first_list->as_list_len; i++) {
        new_list_arr[i] = first_list->as_list[i];
    }
    for(int i = first_list->as_list_len; i < first_list->as_list_len + second_list->as_list_len; i++) {
        new_list_arr[i] = second_list->as_list[i - first_list->as_list_len];
    }
    return lni_new_list(new_list_arr, first_list->as_list_len + second_list->as_list_len);
 }

/**
 * Reverses the list
 * @param list The list
 * @return The list in reverse order
 */
 lni_object* reverse_list(lni_object* list) {
    assert(list->type == LNI_LIST);
    lni_object** new_list_arr = malloc(sizeof(**new_list_arr) * list->as_list_len);
    for(int i = 0; i < first_list->as_list_len; i++) {
        new_list_arr[i] = first_list->as_list[list->as_list_len - i - 1];
    }
    return lni_new_list(new_list_arr, list->as_list_len);
 }

/**
 * Adds an object to the front of the list
 * @param list The list
 * @param object The object to be added to the front
 * @return The list
 */
 lni_object* push_front(lni_object* list, lni_object* object) {
    assert(list->type == LNI_LIST);
    lni_object** new_list_arr = malloc(sizeof(**new_list_arr) * (list->as_list_len + 1));
    new_list_arr[0] = object;
    for(int i = 0; i < list->as_list_len; i++) {
        new_list_arr[i + 1] = list->as_list[i];
    }
    return lni_new_list(new_list_arr, list->as_list_len + 1);
 }

/**
 * Adds an object to the back of the list
 * @param list The list
 * @param object The object to be added to the back
 * @return The list
 */
 lni_object* push_back(lni_object* list, lni_object* object) {
    assert(list->type == LNI_LIST);
    lni_object** new_list_arr = malloc(sizeof(**new_list_arr) * (list->as_list_len + 1));
    for(int i = 0; i < list->as_list_len - 1; i++) {
        new_list_arr[i] = list->as_list[i];
    }
    new_list_arr[list->as_list_len - 1] = object;
    return lni_new_list(new_list_arr, list->as_list_len + 1);
 }

/**
 * Returns the length of the string
 * @param str The string
 * @return Length of the string
 */
 lni_object* strlen(lni_object* str) {
    assert(str->type == LNI_STRING);
    return lni_new_int(strlen(str->as_string));
 }

/**
 * Returns a substring of the string specified by the starting index and ending index
 * @param str The string
 * @param str The starting index of the string (inclusive)
 * @param str The ending index of the string (exclusive)
 * @return The substring
 */
 lni_object* substring(lni_object* str, lni_object* start, lni_object* end) {
    assert(str->type == LNI_STRING);
    assert(start->type == LNI_INT);
    assert(end->type == LNI_INT);
    assert(start->as_int >= 0);
    assert(end->as_int <= strlen(str->as_string));
    assert(end->as_int >= start->as_int);
    char* new_string = malloc(sizeof(char) * (end->as_int + 1 - start->as_int));
    new_string[end->as_int - start->as_int] = 0;
    strncpy(new_string, str->as_string + start->as_int, end->as_int - start->as_int);
    return lni_new_string(new_string);
 }

/**
 * Returns the reverse of a string
 * @param str The string
 * @return The reverse of the string
 */
 lni_object* reverse_string(lni_object* str) {
    assert(str->type == LNI_STRING);
    int string_length = strlen(str->as_string);
    char* new_string = malloc(sizeof(char) * (string_length + 1));
    new_string[string_length] = 0;
    for(int i = 0; i < string_length; i++) {
        new_string[i] = str->as_string[string_length - i - 1];
    }
    return lni_new_string(new_string);
 }

/**
 * Converts a string to an int
 * @param str The string
 * @return The int
 */
 lni_object* convert_to_int(lni_object* str) {
    assert(str->type == LNI_STRING);
    return lni_new_int(atoi(str->as_string));
 }

 /**
 * Generates a random number
 * @return A random number starting from 0
 */
 lni_object* random_number() {
    return lni_new_int(rand());
 }