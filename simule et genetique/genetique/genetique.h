#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef enum { faux, vrai } booleen;

typedef struct {
    int n;       // nombre de sommets
    int nMax;    // nombre max de sommets
    booleen value;   // graphe value ou non
    char** nomS;    // noms des sommets
    int* element; // existence d'un arc (i, j)
    int** valeur;  // cout de l'arc (i, j)
    booleen* marque;  // sommet marque (visite) ou non
} GrapheMat;

typedef struct {
    int chemin[LONGUEUR_INDIVIDU];
    double adaptation;
} Individu;

GrapheMat* creerGrapheMat(int nMax);
void initialiserGraphe(GrapheMat* graphe);
void detruireGraphe(GrapheMat* graphe);
void randomizePopulation(Individu population[], GrapheMat* graphe);
Individu selectionner(Individu population[]);
Individu reproduire(Individu parent1, Individu parent2);
void muter(Individu* individu);
double calculerAdaptation(Individu individu, GrapheMat* graphe);
void afficherIndividu(Individu individu, GrapheMat* graphe);
