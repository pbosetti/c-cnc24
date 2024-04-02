#include "4_struct_union.h"

int main(int argc, const char** argv) {
  // Declarations
  // point_t my_struct;     // Simple declaration
  // Simple pointer declaration and assign NULL value
  point_t* my_struct_pt = NULL; 

  // Struct
  printf("Structs\n");
  // Declare and initialize the structure
  point_t my_struct = {
    .x = 0.0f,
    .y = 1.0f,
    .z = 2.0f
  };

  // Declare and initialize later
  point_t my_struct1; // Declare
  // Let now set struct fields
  my_struct1.x = 3.0f;
  my_struct1.z = 9999.0f;

  // Lets take the pointer to the struct
  printf("my_struct_pt value is: %p\n", my_struct_pt);
  my_struct_pt = &my_struct1;
  printf("my_struct_pt value is: %p\n", my_struct_pt);

  // Lets assign new values to my_struct1 using struct pointer
  my_struct_pt->x = 42.0f;
  my_struct_pt->y = 66.0f;

  // Print content
  print_point(&my_struct1); // The same as print_point(my_struct_pt)

  // Lets print the size of the struct
  printf("Size of point_t struct is: %lu [bytes]\n", sizeof(point_t));

  // Unions
  printf("Unions\n");
  my_union_t my_union;     // Simple declaration
  my_union_t* my_union_pt; // Declare union pointer

  // Set my_union fields
  my_union.x = 100.0f;
  my_union.z = -2000.0f;

  // Assign union poiter address
  my_union_pt = &my_union;

  // Lets print some my_union components
  printf("The x component of my_union is: %f\n", my_union.x);
  printf("The y component of my_union is: %f\n", my_union.y);

  // Lets print the size of union
  printf("The size of my_union_t is: %lu [bytes]\n", sizeof(my_union));

  // Lets print data union field
  print_bytes(my_union.data, sizeof(my_union_t));

  return 0;
}

void print_point(point_t* point) {
  printf("X: %.2f - Y: %.2f - Z: %.2f\n", point->x, point->y, point->z);
}

// Print bytes array given pointer to the array and its size
void print_bytes(unsigned char* array, int size) {
  unsigned int i = 0;
  printf("My array content is: ");
  for(i=0; i<size; i++) {
    printf("0x%02X | ", array[i]);
  }
  printf("\n");
}