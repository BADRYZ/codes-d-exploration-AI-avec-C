#include <limits.h>


typedef int  booleen;
#define faux 0
#define vrai 1
typedef char NomSom[20];  // nom d'un sommet
#define INFINI INT_MAX

typedef int* Matrice;

typedef struct {
  int      n;       // nombre de sommets
  int      nMax;    // nombre max de sommets
  booleen  value;   // graphe value ou non
  NomSom*  nomS;    // noms des sommets
  Matrice  element; // existence d'un arc (i, j)
  Matrice  valeur;  // cout de l'arc (i, j)
  booleen* marque;  // sommet marque (visite) ou non
} GrapheMat;

GrapheMat* creerGrapheMat     (int nMax, int value);
void       detruireGraphe     (GrapheMat* graphe);
void       ajouterUnSommet    (GrapheMat* graphe, NomSom nom);
void       ajouterUnArc       (GrapheMat* graphe, NomSom somD, NomSom somA, int cout);
void       ecrireGraphe       (GrapheMat* graphe);

//exploration deja defenie avec des modif
void       parcoursProfond    (GrapheMat* graphe);

// implementer par moi
static void ecrireEtape (Matrice a, Matrice p, int k, int ns, int nMax);
//FLOYD
static void initFloyd (GrapheMat* graphe, Matrice* a, Matrice* p);
void       floyd              (GrapheMat* graphe);

//TP1
//exploration
void parcoursLargeurD(GrapheMat* graphe);
void parcoursProfendeurLimite(GrapheMat* graphe);
void parcoursIterativeEnprofendeur(GrapheMat* graphe);
