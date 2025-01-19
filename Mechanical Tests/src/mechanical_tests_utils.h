#ifndef MECHANICAL_TESTS_UTILS_H
#define MECHANICAL_TESTS_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>


#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define PI 3.14159265358979323846

typedef struct points_dataset{

    double ** points;

} *Points_Dataset;

typedef struct material {
    double YoungsModulus;
    double PoissonsRatio;
    double YieldStrength;
    double UltimateStrength;
    double Density;
} Material;

typedef enum test_type {
    TENSION,
    COMPRESSION,
    SHEAR,
    BENDING,
    TORSION
} Test_type;

typedef enum type_of_graph {
    TRUE,
    ENGINEERING
} Type_of_graph;

typedef struct mechanical_test
{
    Test_type test_type;
    Material test_material;

    // In milimiters per minute (mm/min)
    double velocity;

    // In milimiters (mm)
    double Initial_Length;
    double Initial_Width;
    double Initial_Thickness;

    double diameter;

} Mechanical_test;


extern const char* test_type_str[];
extern const char* type_of_graph_str[];

//////////////////////////////////////////////////////////
//       Βοηθητικές συναρτήσεις για την Υλοποίηση       //
//////////////////////////////////////////////////////////
Points_Dataset create_dataset(long int number_of_points);

//////////////////////////////////////////
//      Υπολογισμός φυσικών μεγεθών     //
//////////////////////////////////////////
// Βοηθητική συνάρτηση που βρίσκει την ακτίνα καμπυλότητας του βέλους κάμψης
double calculate_radius_of_curvature(double arc_length, Mechanical_test test);

///////////////////////////////////
//       Μηχανικές Δοκιμές       //
///////////////////////////////////
// Κάμψη χάλυβα
void test_1(Type_of_graph type_of_graph);

// Εφελκυσμός χάλυβα
void test_2(Type_of_graph type_of_graph);

// Εφελκυσμός αλουμινίου
void test_3(Type_of_graph type_of_graph);

// Θλίψη αλουμινίου
void test_4(Type_of_graph type_of_graph);


extern Material Steel;
extern Material Aluminum;

#endif // MECHANICAL_TESTS_UTILS_H