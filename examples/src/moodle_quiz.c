#include <stdio.h>
#include <stdlib.h>

// How to compile it:
// 1. Go to examples folder
// 2. gcc src/moodle_quiz.c -o moodle_quiz -g
//    or
//    clang src/moodle_quiz.c -o moodle_quiz -g
//
// ATTENTION: Remember -g (debug) option


// Example quiz from moodle
// Write a function that reverses an array. 
// Follow the given prototype: the function shall change in place the passed array.
// The size of the passed array must be larger than zero.

void reverse(int *ary, unsigned int ary_size) {
    int tmp_value = 0;
    for (int i=0; i != ary_size/2; i++) {
        tmp_value = ary[ary_size -i -1]; // Copy last value
        ary[ary_size -i -1] = ary[i];
        ary[i] = tmp_value;

        for(int i=0; i<ary_size; i++) {
            printf("%i", ary[i]);
        }
        printf("\n");
    }
}

int main() {
    int test_array[6] = {0,1,2,3,4,5};

    reverse(test_array, 6);

    for(int i=0; i<6; i++) {
        printf("%i", test_array[i]);
    }
    printf("\n");

    printf("I'm moodle quiz solution\n");
    return 0;
}