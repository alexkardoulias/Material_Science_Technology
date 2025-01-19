#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "mechanical_tests_utils.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define PI 3.14159265358979323846

const char* test_type_str[] = {
    "Tension",
    "Compression",
    "Shear",
    "Bending",
    "Torsion"
};

const char* type_of_graph_str[] = {
    "True",
    "Engineering"
};

//////////////////////
//  Ορισμός υλικών  // 
//////////////////////
Material Steel = {200e9, 0.3, 250e6, 400e6, 7850};
Material Aluminum = {70e9, 0.33, 50e6, 300e6, 2700};

//////////////////////////////////////////////////////////
//       Βοηθητικές συναρτήσεις για την Υλοποίηση       //
//////////////////////////////////////////////////////////
Points_Dataset create_dataset(long int number_of_points) {
    if(number_of_points == 0) {
        perror("Number of points must be a non-zero value\n");
        return NULL;
    }

    Points_Dataset dataset = malloc(sizeof(struct points_dataset));
    if (dataset == NULL) {
        perror("Memory allocation failed\n");
        return NULL;
    }

    dataset->points = malloc(sizeof(double*)*number_of_points);
    if (dataset->points == NULL) {
        perror("Memory allocation failed\n");
        return NULL;
    }

    for (int i = 0; i < number_of_points; i++) {
        dataset->points[i] = malloc(sizeof(double)*2);
        if (dataset->points[i] == NULL) {
            perror("Memory allocation failed\n");

            // Αποδέσμευση μνήμης
            for (int j = 0; j < i; j++) {
                free(dataset->points[j]);
            }
            free(dataset->points);
            
            return NULL;
        }
    }

    // Αρχικοποίηση των σημείων πάνω στην ευθεία y = 0
    for (int i = 0; i < number_of_points; i++) {
        dataset->points[i][0] = i;
        dataset->points[i][1] = 0;
    }
    
    return dataset;
}

void free_dataset(Points_Dataset dataset, long int number_of_points) {
    if (dataset == NULL) {
        perror("Dataset is NULL\n");
        return;
    }

    for (int i = 0; i < number_of_points; i++) {
        free(dataset->points[i]);
    }
    free(dataset->points);
    free(dataset);
}

//////////////////////////////////////////
//      Υπολογισμός φυσικών μεγεθών     //
//////////////////////////////////////////

// Βοηθητική συνάρτηση που βρίσκει την ακτίνα καμπυλότητας του βέλους κάμψης
// Χρήσιμη για τον προσδιορισμό της πραγματικής τάσης στις δοκιμές κάμψης.
double calculate_radius_of_curvature(double arc_length, Mechanical_test test) {
    // Example using arc_length and an assumed chord length
    double chord_length = test.Initial_Length; // Replace with the correct chord length
    return (pow(arc_length, 2)) / (2 * (arc_length - chord_length));
}

// Υπολογισμός της συμβατικής παραμόρφωσης σε ένα σημείο
// ΟΧΙ επί τοις εκατό
double calculate_point_strain(double elongation, double Initial_Length) {
    if (Initial_Length == 0) {
        perror("Initial_Length must be a non-zero value\n");
        return NAN;
    }

    double strain = (elongation / Initial_Length);

    return strain;
}

// Υπολογισμός της συμβατικής τάσης σε ένα σημείο
double calculate_point_stress(double Force_kN, Mechanical_test test) {
    if (test.Initial_Width == 0 && test.Initial_Thickness == 0 && test.diameter == 0) {
        perror("Zero-dimention object\n");
        return NAN;
    }

    double stress = -1;
    // Υπολογισμός της συμβατικής τάσης για εφελκυσμό
    // Η πραγματική εφελκυστική τάση υπολογίζεται στα αντίστοιχα tests
    if (test.test_type == TENSION) {
        stress = Force_kN / (test.Initial_Width*test.Initial_Thickness);
        return stress * 1e6;
    }

    // Υπολογισμός της συμβατικής τάσης για θλίψη
    // Η πραγματική θλιπτική τάση υπολογίζεται στο αντίστοιχο test
    if (test.test_type == COMPRESSION) {
        stress = Force_kN / (PI*pow(test.diameter, 2))/4;
        return stress * 1e6;
    }
    
    // Υπολογισμός της συμβατικής τάσης για κάμψη
    // Η πραγματική καμπτική τάση υπολογίζεται στο αντίστοιχο test
    if (test.test_type == BENDING) {
        stress = (3*Force_kN*test.Initial_Length) / (2*test.Initial_Width*pow(test.Initial_Thickness, 2));
    }

    return stress * 1e6;
}

///////////////////////////////////
//       Μηχανικές Δοκιμές       //
///////////////////////////////////
// Κάμψη χάλυβα
void test_1(Type_of_graph type_of_graph) {
    Mechanical_test test_1 = {BENDING, Steel, 10, 110, 20, 1, NAN};

    // Δύναμη συναρτήσει της επιμήκυνσης
    double force_to_elongation_1[33][2] = {
        // Βέλος Κάμψης (mm)        Δύναμη (kN)
        {0.7904655679090962,	0.00017594073761679837},
        {1.1292349009706761,	0.005102635788585811},
        {1.3174426131818247,	0.01055718849217512},
        {1.731496547434038, 	0.016011723475879456},
        {1.9949812793049846,	0.020586501611845033},
        {2.2961121025366618,	0.02533723820531232},
        {2.6725237361935585,	0.029560117146159356},
        {3.1995007814662872,	0.03360703762950465},
        {3.8394009377595446,	0.03782990771040923},
        {4.705151106859281,	    0.040469204833453},
        {5.495612884002956, 	0.04275659390143579},
        {6.4742785825923725,	0.0450439741094761},
        {7.377667261521998,	    0.04627565673216084},
        {8.20577892079183,	    0.046627555927279345},
        {9.109163808956042,	    0.04768327123251988},
        {10.01255248788566,	    0.04785923854996408},
        {10.991221977240498,	0.04821112888514014},
        {12.158095388041062,	0.04856304580014361},
        {13.099122576800598,	0.04944281150782486},
        {14.077799647686263,	0.0496187699653266},
        {15.09411143763641,	    0.0496187699653266},
        {16.14805794665104,	    0.04944281150782486},
        {17.54077757949267,	    0.04979471070294338},
        {18.707658571824076,	0.04944281150782486},
        {20.062743485601224,	0.049266853050323134},
        {21.04140539342522, 	0.04873898653776041},
        {22.170644085161317,	0.04856304580014361},
        {23.41280209715252,	    0.04821112888514014},
        {24.993725651439878,	0.04785923854996408},
        {25.972395140794713,	0.04697947284228283},
        {27.402764655762496,	0.045747799079540574},
        {28.720199687413487,	0.045219941426920304},
        {30.03764230059532, 	0.044868033371859316}
    };
    
    // Τάση συναρτήσει της παραμόρφωσης
    // Βέλος κάμψης (mm)     Τάση (kPa)
    double stress_to_strain_1[33][2];

    if (type_of_graph == TRUE) {
        printf("Incomplete function\n");
        sleep(0.5);
        printf("The program can provide only the engineering σ-δ diagram\n");
    }

    // Υπολογισμός της συμβατικής τάσης σε κάθε σημείο
    for (int i = 1; i < 33; i++) {
        stress_to_strain_1[i][0] = force_to_elongation_1[i][0];
        stress_to_strain_1[i][1] = calculate_point_stress(force_to_elongation_1[i][1], test_1);
    }

    // Υπολογισμός της πραγματικής καμπτικής τάσης σε κάθε σημείο
    // INCOMPLETE !!!
    // if (type_of_graph == TRUE) {
    //     double stress_to_strain_1_true[33][2];
    //     for (int i = 1; i < 33; i++) {
    //         ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //         break;
    //     }
    // 
    //     printf("Βέλος κάμψης (mm)     Τάση (kPa)\n");
    //     for(int i = 0; i < 33; i++) {
    //         printf("%f %f\n", stress_to_strain_1_true[i][0], stress_to_strain_1_true[i][1]);
    //     }
    //
    //     return;
    // }

    printf("Βέλος κάμψης (mm)     Τάση (kPa)\n");
    for(int i = 0; i < 33; i++) {
        printf("%f %f\n", stress_to_strain_1[i][0], stress_to_strain_1[i][1]);
    }

    return;
}

// Εφελκυσμός χάλυβα
void test_2(Type_of_graph type_of_graph) {
    // Εφελκυσμός χάλυβα
    Mechanical_test test_2 = {TENSION, Steel, 10, 100, 20, 1, NAN};

    // Δύναμη συναρτήσει της επιμήκυνσης
    double force_to_elongation_2[20][2] = {
        // Επιμήκυνση (mm)        Δύναμη (kN)
        {0.5187307148897219,	2.253424670061397},
        {0.6051847347015271,	3.5958898472370766},
        {1.1239154495912427,	4.4589048737453},
        {1.9380389771949236,	4.881170631120661},
        {2.7665682100911164,	5.2260273095702106},
        {4.409220970590996, 	5.705479289201823},
        {5.39625583582773,	    5.919983863310535},
        {6.397696406356981,	    6.184931268833432},
        {8.213257206480458,     6.472600810544657},
        {10.028818006603936,	6.712326800360463},
        {11.195963764110534,	6.798978549647101},
        {12.622478177071443,	6.904109238280848},
        {13.861672249097332,	6.938819320027706},
        {15.21613834753896,	    7},
        {17.89625253781827,	    7.143835228096653},
        {20.057636013207865,	7.191778779992072},
        {22.56484216386358,	    7.191778779992072},
        {25.158502334331093,	7.191778779992072},
        {27.752162504798605,	7.191778779992072},
        {30.086454019811796,	7.191778779992072}
    };
    
    // Τάση συναρτήσει της παραμόρφωσης
    // Παραμόρφωση (%)     Τάση (kPa)
    double stress_to_strain_2[20][2];

    for (int i = 1; i < 20; i++) {
        stress_to_strain_2[i][0] = calculate_point_strain(force_to_elongation_2[i][0], test_2.Initial_Length);
        stress_to_strain_2[i][1] = calculate_point_stress(force_to_elongation_2[i][1], test_2);
    }

    if (type_of_graph == TRUE) {
        double stress_to_strain_2_true[20][2];
        for (int i = 1; i < 20; i++) {
            stress_to_strain_2_true[i][0] = log(1 + stress_to_strain_2[i][0]);
            stress_to_strain_2_true[i][1] = stress_to_strain_2[i][1]*(1 + stress_to_strain_2[i][0]);
        }

        printf("Παραμόρφωση (%%)     Τάση (kPa)\n");
        for(int i = 0; i < 20; i++) {
            printf("%f %f\n", stress_to_strain_2_true[i][0], stress_to_strain_2_true[i][1]);
        }

        return;
    }

    printf("Παραμόρφωση (%%)     Τάση (kPa)\n");
    for(int i = 0; i < 20; i++) {
        printf("%f %f\n", stress_to_strain_2[i][0], stress_to_strain_2[i][1]);
    }

    return;
}

// Εφελκυσμός αλουμινίου
void test_3(Type_of_graph type_of_graph) {
    Mechanical_test test_3 = {TENSION, Aluminum, 5, 100, 20, 1, NAN};

    double force_to_elongation_3[23][2] = {
        // Επιμήκυνση (mm)        Δύναμη (kN)
        {0.3169793347029427,    0.052313619237728926},
        {0.38382510991675756,   0.4369308721195655},
        {0.4358151879960097,    0.8798228972520057},
        {0.5026609632098231,    1.3343703463380492},
        {0.5843601916081773,    1.7539532842013412},
        {0.7254784686280753,    2.138570537083179},
        {1.0077135267011919,    2.3017417773987976},
        {1.2379585066950547,    2.336707462380253},
        {1.5424759884950008,	2.371672560482356},
        {1.7727202205055226,	2.3600171365287537},
        {2.1292285283680634,    2.383326810677253},
        {2.470881887079381,	    2.3600171365287537},
        {2.7753993688793286,    2.3600171365287537},
        {3.0279260246166815,    2.3017417773987976},
        {3.243315307475983,	    2.2201567441203407},
        {3.4884144886377264,    2.0919506019068272},
        {3.688950318312487,	    1.9171245245169588},
        {3.844922796500267,     1.7073333490249896},
        {3.9563304272343793,    1.3343703463380492},
        {3.986040325536823,	    1.0080296263448727},
        {4.0008952746880455,    0.47189597022166846},
        {4.245994455849788,	    -0.08754736005003652},
        {4.528229513922903,	    -0.08754736005003652}
    };

    // Τάση συναρτήσει της παραμόρφωσης
    // Παραμόρφωση (%)     Τάση (kPa)
    double stress_to_strain_3[23][2];

    for (int i = 1; i < 23; i++) {
        stress_to_strain_3[i][0] = calculate_point_strain(force_to_elongation_3[i][0], test_3.Initial_Length);
        stress_to_strain_3[i][1] = calculate_point_stress(force_to_elongation_3[i][1], test_3);
    }

    if (type_of_graph == TRUE) {
        double stress_to_strain_3_true[23][2];
        for (int i = 1; i < 23; i++) {
            stress_to_strain_3_true[i][0] = log(1 + stress_to_strain_3[i][0]);
            stress_to_strain_3_true[i][1] = stress_to_strain_3[i][1]*(1 + stress_to_strain_3[i][0]);
        }

        printf("Παραμόρφωση (%%)     Τάση (kPa)\n");
        for(int i = 0; i < 23; i++) {
            printf("%f %f\n", stress_to_strain_3_true[i][0], stress_to_strain_3_true[i][1]);
        }

        return;
    }

    printf("Παραμόρφωση (%%)     Τάση (kPa)\n");
    for(int i = 0; i < 23; i++) {
        printf("%f %f\n", stress_to_strain_3[i][0], stress_to_strain_3[i][1]);
    }

    return;
}

// Θλίψη αλουμινίου
void test_4(Type_of_graph type_of_graph) {
    Mechanical_test test_4 = {COMPRESSION, Aluminum, 8, 10, 1, NAN, 10};

    // Δύναμη συναρτήσει της θλιπτικής επιμήκυνσης
    double force_to_elongation_4[22][2] = {
        // Επιμήκυνση (mm)        Δύναμη (kN)
        {0.2009926609363, 	    0},
        {0.3870965323589,	    0},
        {0.4913158098929, 	    6.457553248892196},
        {0.5732011534689,	    12.398511602602522},
        {0.6625308918018,	    19.11437530565435},
        {0.9156335663487,	    22.472320163748975},
        {1.1687347415209,	    25.05534666593335},
        {1.4441683512765,	    26.60517037118517},
        {1.7493795400594,	    28.154981069868295},
        {2.0545907288422,	    29.446494320960483},
        {2.4416872612012,	    31.254615473803288},
        {2.8436725830738,	    34.0959394235786},
        {3.1935486410231,       35.64575012226172},
        {3.5880895681389,	    39.00368197378767},
        {3.9454085214704,	    41.32841102838106},
        {4.2580641050102,	    43.91142452399674},
        {4.6823811117788,	    47.78596427727329},
        {5.0471459592418,	    52.17712493886911},
        {5.3821332276774,	    56.05166469214566},
        {5.7096761013562,	    60.70110979476377},
        {5.9776668157294,	    65.09224444322226},
        {6.1861038714227,	    69.74168954584034}

    };
    
    // Τάση συναρτήσει της παραμόρφωσης
    // Παραμόρφωση (%)     Τάση (kPa)
    double stress_to_strain_4[22][2];

    for (int i = 1; i < 22; i++) {
        stress_to_strain_4[i][0] = calculate_point_strain(force_to_elongation_4[i][0], test_4.Initial_Length);
        stress_to_strain_4[i][1] = calculate_point_stress(force_to_elongation_4[i][1], test_4);
    }

    if (type_of_graph == TRUE) {
        double stress_to_strain_4_true[22][2];
        for (int i = 1; i < 22; i++) {
            stress_to_strain_4_true[i][0] = log(1 + stress_to_strain_4[i][0]);
            stress_to_strain_4_true[i][1] = stress_to_strain_4[i][1]*(1 + stress_to_strain_4[i][0]);
        }

        printf("Παραμόρφωση (%%)     Τάση (kPa)\n");
        for(int i = 0; i < 22; i++) {
            printf("%f %f\n", stress_to_strain_4_true[i][0], stress_to_strain_4_true[i][1]);
        }

        return;
    }

    printf("Παραμόρφωση (%%)     Τάση (kPa)\n");
    for(int i = 0; i < 22; i++) {
        printf("%f %f\n", stress_to_strain_4[i][0], stress_to_strain_4[i][1]);
    }

    return;
}


