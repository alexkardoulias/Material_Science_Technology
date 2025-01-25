#include <stdio.h>
#include <stdlib.h>
#include "miller_indices_utils.h"

const char* grid_system_name[] = {
    "BCC", "FCC", "HCP"
};

const char* triplet_type_name[] = {
    "AXIS INTERCEPTS", "MILLER INDICES"
};

const char* miller_indices_type_name[] = {
    "NONE", "SURFACE", "DIRECTION"
};

Triplet triplet_create(Triplet_Type triplet_type, Miller_Indices_Type miller_indices_type, Global_Pointer_Array pointers_addresses)
{
    Triplet triplet = (Triplet)malloc(sizeof(struct triplet));
    if (triplet == NULL) {
        perror("Could not allocate memory for a struct triplet\n");
        return NULL;
    } else {
        if (pointers_addresses->capacity < pointers_addresses->size) {
            pointers_addresses->pointer_array[pointers_addresses->capacity] = (void*)triplet;
            pointers_addresses->capacity++;
        } else {
            pointers_addresses->pointer_array = realloc(pointers_addresses->pointer_array, (pointers_addresses->size)*2);
        }
    }

    triplet->miller_indices_type = miller_indices_type;
    triplet->triplet_type = triplet_type;

    return triplet;
}

void triplet_set_values(Triplet triplet, double a, double b, double c)
{
    triplet->x_point = a;
    triplet->y_point = b;
    triplet->z_point = c;

    return;
}

void triplet_print_values (Triplet triplet)
{
    if (triplet->triplet_type == 0 || triplet->triplet_type == 1) {
        printf("Type of information: %s\n", triplet_type_name[triplet->triplet_type]);
    }

    if (triplet->miller_indices_type != NONE) { 
        printf("Type of information: %s\n", miller_indices_type_name[triplet->miller_indices_type]);
    }

    printf("%f %f %f\n", triplet->x_point, triplet->y_point, triplet->z_point);
    
    return;
}

void free_heap_allocated_memory(Global_Pointer_Array pointers_addresses)
{
    for (int i =0; i <pointers_addresses->capacity; i++) {
        free(pointers_addresses->pointer_array[i]);
    }

    free(pointers_addresses->pointer_array);
    free(pointers_addresses);

    return;
};

int greatest_common_divisor(int a, int b)
{
    if (b == 0)
        return a;

    return greatest_common_divisor(b, a % b);
}

const char* get_crystalic_system_name(Metal_Grid_System system) 
{
    return grid_system_name[system];
}

int get_crystalic_system(void)
{
    int enum_number = -1;
    char metal_grid_system[10];  // Allocate space for the input string

    printf("Enter metal grid system (BCC/FCC/HCP): \n");
    scanf("%s", metal_grid_system);

    // For BCC, FCC, or HCP
    if ((metal_grid_system[0] & ~('A' ^ 'a')) == 'B') {
        enum_number = BCC;
    } else if ((metal_grid_system[0] & ~('A' ^ 'a'))== 'F') {
        enum_number = FCC;
    } else if ((metal_grid_system[0] & ~('A' ^ 'a'))== 'H') {
        enum_number = HCP;
    } else {
        printf("Unidentifiable crystal lattice system.\n");
    }

    return enum_number;
}

Triplet get_axis_intercept_coordinates(Triplet axis_intercept_coordinates)
{
    printf("Enter x intercept -> ");
    scanf("%lf", &(axis_intercept_coordinates->x_point));
    
    printf("Enter y intercept -> ");
    scanf("%lf", &(axis_intercept_coordinates->y_point));
    
    printf("Enter z intercept -> ");
    scanf("%lf", &(axis_intercept_coordinates->z_point));

    putchar('\n');

    return axis_intercept_coordinates;
}

// 
// Axis intercept coordinates should have exactly two 0 or none!
// 
Triplet calculate_miller_indices(Triplet axis_intercept_coordinates, Global_Pointer_Array pointers_addresses){
    double temp_a=0, temp_b=0, temp_c=0;
    double result_a, result_b, result_c;

    if (axis_intercept_coordinates->x_point != 0)   temp_a = 1/axis_intercept_coordinates->x_point;
    if (axis_intercept_coordinates->y_point != 0)   temp_b = 1/axis_intercept_coordinates->y_point;
    if (axis_intercept_coordinates->z_point != 0)   temp_c = 1/axis_intercept_coordinates->z_point;

    if (temp_b != 0 && temp_c != 0) result_a = (temp_b*temp_c); else result_a = temp_a;
    if (temp_a != 0 && temp_c != 0) result_b = (temp_a*temp_c); else result_b = temp_b;
    if (temp_a != 0 && temp_b != 0) result_c = (temp_a*temp_b); else result_c = temp_c;

    int gcd = greatest_common_divisor(result_a, result_b);
    gcd = greatest_common_divisor(gcd, result_c);

    if (gcd != 0) {
        result_a = result_a/gcd;
        result_b = result_b/gcd;
        result_c = result_c/gcd;
    }

    Triplet triplet = triplet_create(MILLER_INDICES, SURFACE, pointers_addresses);
    if (triplet == NULL) {
        perror("Failed memory allocation in function: miller_indeces_surface()\n");
        return NULL;
    }

    triplet_set_values(triplet, result_a, result_b, result_c);

    return triplet;
}

