#include <stdio.h>
#include <stdlib.h>

// Define read buffer length
#define BUFFER_LENGTH (20U)

// enum: lists the possible return values
// note: enum values are internally stored as ints. Typically, the first value
// is 0, the second 1 and so on, BUT THIS IT NOT GRANTED (different compilers
// can behave differently). If the numeric value is important (e.g. because you
// plan to transmit it via network), then it is safer to force its values. This
// can be done by explicitly give the value of 0 to the first element.
enum error {
    ERROR_SUCCESS = 0,
    ERROR_WRONG_ARGC,
    ERROR_OPEN,
    ERROR_WRITE,
};

// Define application error_t type as enum error
typedef enum error errors_t;

// Single step way
// typedef enum error {
//   ERROR_SUCCESS = 0,
//   ERROR_WRONG_ARGC,
//   ERROR_OPEN,
//   ERROR_WRITE,
// } errors_t;

int main(int argc, char** argv) {
    // Declarations
    errors_t app_error = ERROR_SUCCESS;
    char* filename;

    // 1. Check if I have at least 2 arguments
    if(argc != 2) {
        fprintf(stderr, "I need 2 arguments but I got %i!\n", argc);
        app_error = ERROR_WRONG_ARGC;
        goto end; // Jump to "end" label
    }

    // 2. Assign filename value 
    filename = argv[1];
    fprintf(stdout, "My filename is: %s\n", filename);

    // 3. Open and write to this file */
    FILE* f = fopen(filename, "w"); // w=write, r=read 

    // 4. Check if file is open
    if(!f) {
        fprintf(stderr, "Filaname: %s is not open!\n", filename);
        app_error = ERROR_OPEN;
        return app_error;
    }

    // 5. Lets write something 
    fprintf(f, "I'm a line\n");
    fprintf(f, "I'm another line\n");
    fprintf(f, "The pi value is: %f\n", 3.14159f);

    // 6. Close the file 
    fclose(f);

    // 7. Lets now read the same file
    FILE* f1 = fopen(filename, "r");

    // 8. Check if file is open
    if(!f1) {
        fprintf(stderr, "Filaname: %s is not open!\n", filename);
        app_error = ERROR_OPEN;
        return app_error;
    }

    // 9. Read file line by line 
    fprintf(stdout, "File content:\n");
    char buffer[BUFFER_LENGTH]; // Used to store tmp file line
    while(fgets(buffer, BUFFER_LENGTH, f1)) {
        fprintf(stdout, "%s", buffer);
    }

    // 10. Close the file
    fclose(f1);

end:
    return app_error;
}