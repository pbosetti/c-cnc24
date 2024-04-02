#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Lets define a new type
typedef double data_t;

// Struct simple definiton
// struct {
//   data_t x, y, z;
// };

// Struct definition as new type with struct name
// typedef struct point {
//   data_t x, y, z;
// } point_t;

// Struct definition as new type
typedef struct __attribute__ ((packed)) {
  data_t x, y, z; //
  uint16_t tmp; // Occupy 8 bytes (on 64bit architecture)
} point_t;

// Union example of type converison
// typedef union test {
//   uint8_t u8_data[4];   // 4 * 1 bytes (sizeof uint8_t)
//   uint16_t u16_data[2]; // 2 * 2 bytes (sizeof uint16_t)
//   uint32_t u32_data;    // 1 * 4 bytes (sizeof uint32_t)
// } test_t;

// Union
typedef union {
  float x, y, z;                     // Size of float is 4 bytes
  unsigned char data[sizeof(float)]; // Array of 4 characters
} my_union_t;

// Function that prints point content
void print_point(point_t* point);

// Function in order to print my_union_t content
void print_bytes(unsigned char* array, int size);

