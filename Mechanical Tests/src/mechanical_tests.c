#include <stdio.h>
#include <unistd.h>

#include "mechanical_tests_utils.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define PI 3.14159265358979323846


///////////////////////////////////////////////
//      Υλοποίηση διεπαφής προγράμματος      //
///////////////////////////////////////////////
int main() {
    // Default type of graph: Engineering
    Type_of_graph type_of_graph = ENGINEERING;
    char type_of_graph_str[20];

    printf("Μάθημα: Μεταλλικά Τεχνικά Υλικά\n");
    printf("Εργαστηριακές Ασκήσεις - Μηχανικές Δοκιμές\n");
    printf("------------------------\n");
    sleep(1);
    printf("Ομάδα 1\n");
    printf("Ονοματεπώνυμο: Αλέξανδρος Καρδούλιας\n");
    printf("------------------------\n");
    sleep(1);

    printf("Επιλέξτε τον τύπο γραφήματος πραγματικό ή συμβατικό:\n");
    sleep(1);
    printf("------------------------\n");
    printf("Για πραγματικό διάγραμμα: true\n");
    printf("Για συμβατικό διάγραμμα: engineering\n");
    printf("------------------------\n");
    sleep(1);
    printf("Enter the graph type (max 20 characters): ");
    if (scanf("%20s", type_of_graph_str) != 1) {
        perror("Error: Failed to read graph type.\n");
        return EXIT_FAILURE; // Exit with failure if input is invalid
    }


    if (strcmp(type_of_graph_str, "true") == 0) {
        type_of_graph = TRUE;
    } else if (strcmp(type_of_graph_str, "engineering") == 0) {
        type_of_graph = ENGINEERING;
    } else {
        perror("Invalid type of graph\n");
        return EXIT_FAILURE;
    }

    
    printf("------------------------\n");


    printf("Μηχανικές δοκιμές\n");
    sleep(1);
    printf("------------------------\n");
    printf("Test 1: Κάμψη χάλυβα            (1)\n");
    printf("Test 2: Εφελκυσμός χάλυβα       (2)\n");
    printf("Test 3: Εφελκυσμός αλουμινίου   (3)\n");
    printf("Test 4: Θλίψη αλουμινίου        (4)\n");
    printf("------------------------\n");
    sleep(1);

    int test_number = -1;
    printf("Enter the test number: ");
    if (scanf("%d", &test_number) != 1) {
        perror("Error: Failed to read test number.\n");
        return EXIT_FAILURE; // Exit with failure if input is invalid
    }

    printf("------------------------\n");
    printf("Showing results for %s diagram, test No %d\n", type_of_graph_str, test_number);
    sleep(1);
    printf("------------------------\n");

    switch (test_number) {
        case 1:
            // Κάμψη χάλυβα
            printf("     Κάμψη χάλυβα\n");
            printf("------------------------\n");
            test_1(type_of_graph);
            break;
        case 2:
            // Εφελκυσμός χάλυβα
            printf("   Εφελκυσμός χάλυβα\n");
            printf("------------------------\n");
            test_2(type_of_graph);
            break;
        case 3:
            // Εφελκυσμός αλουμινίου
            printf(" Εφελκυσμός αλουμινίου\n");
            printf("------------------------\n");
            test_3(type_of_graph);
            break;
        case 4:
            // Θλίψη αλουμινίου
            printf("    Θλίψη αλουμινίου\n");
            printf("------------------------\n");
            test_4(type_of_graph);
            break;
        default:
            // Μη έγκυρος αριθμός/χαρακτήρας
            printf("Invalid test number\n");
            break;
    }

    printf("------------------------\n");

    return EXIT_SUCCESS;
}