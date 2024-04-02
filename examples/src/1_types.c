#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Macros section
#define CHAR_ARR_LENGTH (12U)     // Preprocessor constant
#define _2_POW_N(n) (1U << (n))   // Preprocessor basic math operation
#define COMPILE_SWITCH_OPTION (0) // 0=char* , 1=char[]

// MAIN FUNCTION
// The mandatory main function must:
// - return an integer value (int): 0 means success, any other value failure
// - two arguments: the number of command line arguments and a vector of strings
//   holding the command line arguments
int main(int argc, char* argv[]) {
    // NOTE: exceptionally, and for the sake of clarity, here we will declare
    // variables when needed; in real applications it is preferable to declare
    // variables at the beginning of each block, for some compilers (notably
    // Visual Studio for C) do not accept late declarations.
    unsigned int i;

    // Integers
    printf("Integers\n");
    int my_int = 100; /* Declaration and assignment */
    printf("Size of int is: %lu [bytes]\n", sizeof(int));
    printf("The range of int is: [%d, %d]\n", INT_MIN, INT_MAX);

    // Unsigned integer
    unsigned int my_uint = 99;
    printf("Size of unsigned int is: %lu [bytes]\n", sizeof(my_uint));
    printf("The range of unsigned int is: [%u, %u]\n", 0U, UINT_MAX);

    // Doubles and flaot
    printf("\nDoubles, long doubles and floats\n");
    double my_double = 0.0f;
    printf("Size of double is: %lu\n", sizeof(double));
    printf("The double range is: [%e, %e] and the epsilon is %e\n", DBL_MIN, DBL_MAX, DBL_EPSILON);

    // Long double
    long double my_long_double = 0;
    printf("Size of long double is: %lu\n", sizeof(long double));
    printf("The long double range is: [%Le, %Le] and the epsilon is %Le\n", LDBL_MIN, LDBL_MAX, LDBL_EPSILON);

    // Float
    float my_float = -1.0f;
    printf("Size of float is: %lu\n", sizeof(float));
    printf("The float range is: [%e, %e] and the epsilon is %e\n", FLT_MIN, FLT_MAX, FLT_EPSILON);

    // Boolean 
    printf("\nBoolean\n");
    bool my_boolean = true;
    printf("Size of bool is: %lu [bytes]\n", sizeof(bool));

    // Other integer types
    printf("\nOther integer types\n");

    // uint8
    uint8_t my_u8 = 66;
    printf("Size of uint8_t is: %lu [bytes]\n", sizeof(my_u8));
    printf("The range of uint8_t is: [%u, %u]\n", 0U, UINT8_MAX);

    // int8
    int8_t my_i8 = 55;
    printf("Size of int8_t is: %lu [bytes]\n", sizeof(my_i8));
    printf("The range of int8_t is: [%d, %d]\n", INT8_MIN, INT8_MAX);

    // uint32
    uint32_t my_u32 = 99999;
    printf("Size of uint32_t is: %lu [bytes]\n", sizeof(my_u32));
    printf("The range of uint32_t is: [%u, %u]\n", 0U, UINT32_MAX);

    // int32
    int32_t my_i32 = 99999;
    printf("Size of int32_t is: %lu [bytes]\n", sizeof(my_i32));
    printf("The range of int32_t is: [%d, %d]\n", INT32_MIN, INT32_MAX);

    // Characters and strings
    printf("\nChars\n");
    char my_char = 'c';
    printf("Size of char is: %lu [bytes]\n", sizeof(my_char));
    printf("The range of char is: [%d, %d]\n", CHAR_MIN, CHAR_MAX);

    printf("\nString[]\n");
#if (COMPILE_SWITCH_OPTION == 1)
    // Char[] or strings
    char my_str[CHAR_ARR_LENGTH] = "Pippo"; // String with defined size
    printf("Size of char[] is: %lu [bytes]\n", sizeof(my_str));
    printf("String lenght is: %lu\n", strlen(my_str));
    printf("My string content is: %s\n", my_str);
    printf("First char is: %c\n", my_str[0]);
#else
    char* my_str = "Pluto"; // String as pointer to the first character
    printf("Size of char* is: %lu [bytes]\n", sizeof(*my_str));
    printf("String lenght is: %lu\n", strlen(my_str));
    printf("My string content is: %s\n", my_str);
    printf("First char is: %c\n", (*my_str));
#endif /* COMPILE_SWITCH_OPTION */

    // String print
    printf("String content:\n");
    printf("Chars: ");
    for(i=0; i<sizeof(my_str); i++) {
        printf("|%2c", my_str[i] ? my_str[i] : ' ');
    }
    printf("|\n");

    // Lets print ascii value
    printf("Ascii: ");
    for(i=0; i<sizeof(my_str); i++) {
        printf("|%02X", my_str[i]);
    }
    printf("|\n");

    // Macro example
    printf("\nMacro example:\n");
    unsigned int n = 12;
    printf("2 ^ %u = %u\n", n, _2_POW_N(n));

    return 0;
}
