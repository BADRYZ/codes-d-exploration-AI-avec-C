#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum { faux, vrai } booleen;

typedef struct {
    int n;       // nombre de sommets
    int nMax;    // nombre max de sommets
    booleen value;   // graphe value ou non
    char** nomS;    // noms des sommets
    int* element; // existence d'un arc (i, j)
    int* valeur;  // cout de l'arc (i, j)
    booleen* marque;  // sommet marque (visite) ou non
} GrapheMat;

GrapheMat* creerGrapheMat(int nMax, booleen value);
void detruireGraphe(GrapheMat* graphe);
void initialiserGraphe(GrapheMat* graphe);
double calculerEnergie(int* solution, GrapheMat* graphe);
void swap(int* a, int* b);
void simulatedAnnealing(GrapheMat* graphe);

