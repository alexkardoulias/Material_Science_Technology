#include <stdio.h>
#include <stdlib.h>
#include "miller_indices_utils.h"

#define POINTER_ARRAY_INITIAL_SIZE 10

int main(void)
{
    Global_Pointer_Array pointers_addresses = malloc(POINTER_ARRAY_INITIAL_SIZE);
    if (pointers_addresses == NULL) {
        perror("Failed to allocate memory for pointers_addresses\n");
        return 1;
    } else {
        pointers_addresses->pointer_array = malloc(POINTER_ARRAY_INITIAL_SIZE);
        pointers_addresses->size = POINTER_ARRAY_INITIAL_SIZE;
        pointers_addresses->capacity = 0;
    }


    // Get the metal grid system
    int enum_number = get_crystalic_system();
    if (enum_number == -1) {
        printf("Invalid metal grid system. Exiting program.\n");
        return 1;
    }

    Triplet axis_intercept_coordinates = triplet_create(AXIS_INTERCEPTS, NONE, pointers_addresses);
    if (axis_intercept_coordinates == NULL) {
        printf("Failed to allocate memory for axis_intercept_coordinates\n");
        return 1; // Exit if memory allocation fails
    }

    // Get the coordinates (x, y, z intercepts)
    axis_intercept_coordinates = get_axis_intercept_coordinates(axis_intercept_coordinates);

    // Output for debugging
    printf("\ncrystalic system name: %s\n", get_crystalic_system_name(enum_number));
    printf("axis intercept coordinates: (%lf, %lf, %lf)\n", axis_intercept_coordinates->x_point, axis_intercept_coordinates->y_point, axis_intercept_coordinates->z_point);

    // Calculate miller indices
    Triplet miller_indices = calculate_miller_indices(axis_intercept_coordinates, pointers_addresses);
    if (miller_indices == NULL) {
        perror("Failed to allocate memory for miller_indices\n");
        return 1;
    }

    triplet_print_values(miller_indices);

    free_heap_allocated_memory(pointers_addresses);

    return 0;
}
