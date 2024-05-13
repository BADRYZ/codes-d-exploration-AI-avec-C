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

GrapheMat* creerGrapheMat(int nMax, booleen value) {
    GrapheMat* graphe = (GrapheMat*)malloc(sizeof(GrapheMat));
    if (graphe == NULL) return NULL;
    graphe->n = 0;
    graphe->nMax = nMax;
    graphe->value = value;
    graphe->nomS = malloc(sizeof(char*) * nMax);
    graphe->element = malloc(sizeof(int) * nMax * nMax);
    graphe->valeur = malloc(sizeof(int) * nMax * nMax);
    graphe->marque = malloc(sizeof(booleen) * nMax);
    if (!graphe->nomS || !graphe->element || !graphe->valeur || !graphe->marque) {
        free(graphe->nomS);
        free(graphe->element);
        free(graphe->valeur);
        free(graphe->marque);
        free(graphe);
        return NULL;
    }
    memset(graphe->element, 0, sizeof(int) * nMax * nMax);
    memset(graphe->valeur, 0, sizeof(int) * nMax * nMax);
    memset(graphe->marque, faux, sizeof(booleen) * nMax);
    return graphe;
}

void detruireGraphe(GrapheMat* graphe) {
    if (graphe != NULL) {
        free(graphe->nomS);
        free(graphe->element);
        free(graphe->valeur);
        free(graphe->marque);
        free(graphe);
    }
}

void initialiserGraphe(GrapheMat* graphe) {
    char* noms[] = {"A", "B", "C", "D", "E", "F", "G"};
    int distances[][7] = {
        {0, 121, 263, 103, 95, 136, 57},
        {121, 0, 277, 114, 179, 48, 138},
        {263, 277, 0, 173, 183, 235, 207},
        {103, 114, 173, 0, 85, 87, 64},
        {95, 179, 183, 85, 0, 167, 44},
        {136, 48, 235, 87, 167, 0, 132},
        {57, 138, 207, 64, 44, 132, 0}
    };
    int i, j;
    for (i = 0; i < 7; i++) {
        graphe->nomS[i] = strdup(noms[i]);
        for (j = 0; j < 7; j++) {
            graphe->element[i * graphe->nMax + j] = distances[i][j] > 0 ? vrai : faux;
            graphe->valeur[i * graphe->nMax + j] = distances[i][j];
        }
    }
    graphe->n = 7;
}

double calculerEnergie(int* solution, GrapheMat* graphe) {
    double totalCost = 0;
    for (int i = 0; i < graphe->n - 1; i++) {
        int from = solution[i];
        int to = solution[i + 1];
        if (graphe->element[from * graphe->nMax + to]) {
            totalCost += graphe->valeur[from * graphe->nMax + to];
        }
    }
    return totalCost;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void simulatedAnnealing(GrapheMat* graphe) {
    double T = 1000.0;
    double T_min = 1.0;
    double alpha = 0.95;
    int currentSolution[graphe->n];
    for (int i = 0; i < graphe->n; i++) {
        currentSolution[i] = i;
    }
    double currentEnergy = calculerEnergie(currentSolution, graphe);

    while (T > T_min) {
        int iter = 0;
        int max_iter = (graphe->n / 2) * (graphe->n / 2);
        while (iter < max_iter) {
            int newSolution[graphe->n];
            memcpy(newSolution, currentSolution, sizeof(int) * graphe->n);
            int idx1 = rand() % graphe->n;
            int idx2 = rand() % graphe->n;
            swap(&newSolution[idx1], &newSolution[idx2]);

            double newEnergy = calculerEnergie(newSolution, graphe);
            double energyChange = newEnergy - currentEnergy;

            if (energyChange < 0 || (exp(-energyChange / T) > ((double)rand() / (double)RAND_MAX))) {
                memcpy(currentSolution, newSolution, sizeof(int) * graphe->n);
                currentEnergy = newEnergy;
            }
            iter++;
        }
        T *= alpha;
    }

    printf("Optimal solution found with energy: %f\n", currentEnergy);
    for (int i = 0; i < graphe->n; i++) {
        printf("%s -> ", graphe->nomS[currentSolution[i]]);
    }
    printf("\n");
}

int main() {
    GrapheMat* g = creerGrapheMat(10, vrai);
    if (g == NULL) {
        printf("Failed to create graph.\n");
        return 1;
    }
    initialiserGraphe(g);
    simulatedAnnealing(g);
    detruireGraphe(g);
    return 0;
}
