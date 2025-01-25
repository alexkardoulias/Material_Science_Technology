#ifndef MILLER_INDICES_UTILS_H
#define MILLER_INDICES_UTILS_H

typedef void* Pointer;

typedef enum metal_grid_system { BCC, FCC, HCP } Metal_Grid_System;
typedef enum triplet_type { AXIS_INTERCEPTS, MILLER_INDICES } Triplet_Type;
typedef enum miller_indices_type {NONE, SURFACE, DIRECTION } Miller_Indices_Type;

typedef struct global_pointer_array {
    Pointer *pointer_array;
    int size;
    int capacity;

}*Global_Pointer_Array;

typedef struct triplet {
    Triplet_Type triplet_type;
    Miller_Indices_Type miller_indices_type;

    double x_point;
    double y_point;
    double z_point;
} *Triplet;

// Function declarations

// 
// More documentation is needed
// 
Triplet triplet_create(Triplet_Type triplet_type, Miller_Indices_Type miller_indices_type, Global_Pointer_Array pointers_addresses);

// 
// More documentation is needed
// 
void triplet_set_values(Triplet triplet, double a, double b, double c);

// 
// More documentation is needed
// 
void triplet_print_values (Triplet triplet);

// 
// More documentation is needed
// 
void free_heap_allocated_memory(Global_Pointer_Array pointers_addresses);

// 
// More documentation is needed
// 
int greatest_common_divisor(int a, int b);

// 
// More documentation is needed
// 
const char* get_crystalic_system_name(Metal_Grid_System system);

// 
// More documentation is needed
// 
int get_crystalic_system(void);

// 
// More documentation is needed
// 
Triplet get_axis_intercept_coordinates(Triplet axis_intercept_coordinates);

// 
// More documentation is needed
// 
Triplet calculate_miller_indices(Triplet axis_intercept_coordinates, Global_Pointer_Array pointers_addresses);

#endif
