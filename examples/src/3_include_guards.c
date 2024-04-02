#include "3_include_guards.h"

int main(int argc, const char** argv) {
  // Declerations

  // Check argc > 1
  if(argc == 1) {
    printf("I need more than 1 arguments!\n");
  }

  // Call the function
  print_arguments(argc, argv);

  return 0;
}

void print_arguments(int argc, const char** argv) {
  unsigned int i = 0;
  for(i=0; i<argc; i++) {
    printf("%u - %s\n", i, argv[i]); // argv[i] == (char * string) number i
  }
}