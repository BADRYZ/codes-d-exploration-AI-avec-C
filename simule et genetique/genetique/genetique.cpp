#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

GrapheMat* creerGrapheMat(int nMax) {
    GrapheMat* g = (GrapheMat*)malloc(sizeof(GrapheMat));
    g->nMax = nMax;
    g->nomS = (char**)malloc(nMax * sizeof(char*));
    g->valeur = (int**)malloc(nMax * sizeof(int*));
    for (int i = 0; i < nMax; i++) {
        g->valeur[i] = (int*)malloc(nMax * sizeof(int));
    }
    return g;
}

void initialiserGraphe(GrapheMat* graphe) {
    char* names[] = {"A", "B", "C", "D", "E", "F", "G"};
    int distances[7][7] = {
        {0, 121, 263, 103, 95, 136, 57},
        {121, 0, 277, 114, 179, 48, 138},
        {263, 277, 0, 173, 183, 235, 207},
        {103, 114, 173, 0, 85, 87, 64},
        {95, 179, 183, 85, 0, 167, 44},
        {136, 48, 235, 87, 167, 0, 132},
        {57, 138, 207, 64, 44, 132, 0}
    };
    for (int i = 0; i < 7; i++) {
        graphe->nomS[i] = strdup(names[i]);
        for (int j = 0; j < 7; j++) {
            graphe->valeur[i][j] = distances[i][j];
        }
    }
    graphe->n = 7;
}

void detruireGraphe(GrapheMat* graphe) {
    for (int i = 0; i < graphe->n; i++) {
        free(graphe->nomS[i]);
        free(graphe->valeur[i]);
    }
    free(graphe->nomS);
    free(graphe->valeur);
    free(graphe);
}

void randomizePopulation(Individu population[], GrapheMat* graphe) {
    for (int i = 0; i < TAILLE_POPULATION; i++) {
        for (int j = 0; j < LONGUEUR_INDIVIDU; j++) {
            population[i].chemin[j] = j; // Start with a sequential path
        }
        // Randomly shuffle the path to create a random solution
        for (int j = 0; j < LONGUEUR_INDIVIDU; j++) {
            int r = rand() % LONGUEUR_INDIVIDU;
            int temp = population[i].chemin[j];
            population[i].chemin[j] = population[i].chemin[r];
            population[i].chemin[r] = temp;
        }
        population[i].adaptation = calculerAdaptation(population[i], graphe);
    }
}

Individu selectionner(Individu population[]) {
    double totalFitness = 0;
    for (int i = 0; i < TAILLE_POPULATION; i++) {
        totalFitness += population[i].adaptation;
    }
    double slice = (double)rand() / RAND_MAX * totalFitness;
    double sum = 0;
    for (int i = 0; i < TAILLE_POPULATION; i++) {
        sum += population[i].adaptation;
        if (sum >= slice) {
            return population[i];
        }
    }
    return population[0];
}

Individu reproduire(Individu parent1, Individu parent2) {
    Individu child;
    int c = rand() % LONGUEUR_INDIVIDU;
    memcpy(child.chemin, parent1.chemin, c * sizeof(int));
    memcpy(child.chemin + c, parent2.chemin + c, (LONGUEUR_INDIVIDU - c) * sizeof(int));
    return child;
}

void muter(Individu* individu) {
    int idx1 = rand() % LONGUEUR_INDIVIDU;
    int idx2 = rand() % LONGUEUR_INDIVIDU;
    int temp = individu->chemin[idx1];
    individu->chemin[idx1] = individu->chemin[idx2];
    individu->chemin[idx2] = temp;
}

double calculerAdaptation(Individu individu, GrapheMat* graphe) {
    double cost = 0;
    for (int i = 0; i < LONGUEUR_INDIVIDU - 1; i++) {
        cost += graphe->valeur[individu.chemin[i]][individu.chemin[i + 1]];
    }
    if (cost == 0) return 0; // Avoid division by zero
    return 1.0 / cost; // Using inverse of path cost as fitness score
}

void afficherIndividu(Individu individu, GrapheMat* graphe) {
    for (int i = 0; i < LONGUEUR_INDIVIDU; i++) {
        printf("%s ", graphe->nomS[individu.chemin[i]]);
    }
    printf("with adaptation: %f\n", individu.adaptation);
}
