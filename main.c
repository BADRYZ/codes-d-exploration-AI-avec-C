#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphemat.h"
#include "liregraphe.h"

typedef GrapheMat Graphe;


// include

typedef char NomSom [20];
int c;   // un caract�re lu en avance dans lireUnMot

// ignorer les blancs
void lireBlancs (FILE* fe) {
  while ( ( (c==' ')  || (c=='\n') || (c==13) )  && !feof(fe) ) {
    c = getc(fe);
  }
}

// lire un nom de sommet en ignorant les espaces
void lireUnMot (FILE* fe, char* chaine) {
  char* pCh = chaine;
  lireBlancs (fe); // blancs avant le mot
  while ( isalpha(c) || isdigit(c) ) {
    *pCh++ = (char) c;
    c = getc(fe);
  }
  *pCh = 0;
  lireBlancs (fe);  // blancs apr�s le mot
}

// fournir un pointeur sur un graphe construit
// � partir d'un fichier fe de donn�es
// value = vrai si le Graphe est valu�
GrapheMat* lireGraphe (FILE* fe, int nMaxSom) {
  booleen value = faux;

  GrapheMat* graphe = creerGrapheMat (nMaxSom, faux);

  // lire les noms des sommets
  c = getc(fe);
  while (c != ';') {
    char* somD = (char*) malloc (20);
    lireUnMot (fe, somD);
    ajouterUnSommet (graphe, somD);
  }
  while (c != EOF) {
    c = getc(fe);
    NomSom somD;
    lireUnMot (fe, somD); // lit le sommet de d�part
    if (c != ':') {
      if (c != EOF) printf ("Manque : %c (%d)\n", c,c);
      graphe->value = value;
      return graphe;
    }
    c = getc(fe);
    while (c != ';') {
      NomSom somA;
      lireUnMot (fe, somA); // lit les sommets d'arriv�e
      int cout;
      if (c == '(') {
        value = vrai; // si sommet suivi de ( : S1(25)
        fscanf (fe, "%d", &cout);
        c = getc (fe);
        if (c != ')') printf ("Manque )\n");
        c = getc (fe);
        lireBlancs (fe);
      } else {
        cout = 0;
      }
      ajouterUnArc (graphe, somD, somA, cout);
    }
  }
  graphe->value = value;
  return graphe;
}


// remise � z�ro du tableau de marquage
static void initMarque (GrapheMat* graphe)
{

    for (int i=0; i<graphe->n; i++) graphe->marque [i] = faux;
}
static booleen  grapheDetruit = vrai;
GrapheMat* creerGrapheMat (int nMax, int value)
{
    // allocation de graphe
    GrapheMat* graphe  = (GrapheMat*) malloc (sizeof (GrapheMat));
    graphe->n       = 0;
    graphe->nMax    = nMax;
    graphe->value   = value; // value : vrai si le graphe est valu�
    graphe->nomS    = (NomSom*)  malloc (sizeof(NomSom)  *nMax);
    graphe->marque  = (booleen*) malloc (sizeof(booleen) *nMax);
    graphe->element = (int*)     malloc (sizeof(int)*nMax*nMax);
    graphe->valeur  = (int*)     malloc (sizeof(int)*nMax*nMax);
    // initialisation par d�faut
    for (int i=0; i<nMax; i++)
    {
        for (int j=0; j<nMax; j++)
        {
            graphe->element [i*nMax+j] = faux;
            graphe->valeur  [i*nMax+j] = INFINI;
        }
    }
    initMarque (graphe);
    grapheDetruit = faux;
    return graphe;
}


// d�sallocation d'un graphe
void detruireGraphe (GrapheMat* graphe)
{
    free (graphe->nomS);
    free (graphe->marque);
    free (graphe->element);
    free (graphe->valeur);
    free (graphe);
    grapheDetruit = vrai;
}

static int rang (GrapheMat* graphe, NomSom nom)
{
    int      i      = 0;
    booleen  trouve = faux;
    while ( (i<graphe->n) && !trouve)
    {
        trouve = strcmp (graphe->nomS [i], nom) == 0;
        if (!trouve) i++;
    }
    return trouve ? i : -1;
}

void ajouterUnSommet (GrapheMat* graphe, NomSom nom)
{
    if (rang (graphe, nom) == -1)
    {
        if (graphe->n < graphe->nMax)
        {
            strcpy (graphe->nomS [graphe->n++], nom);
        }
        else
        {
            printf ("\nNombre de sommets > %d\n", graphe->nMax);
        }
    }
    else
    {
        printf ("\n%s d�j� d�fini\n", nom);
    }
}

void ajouterUnArc (GrapheMat* graphe, NomSom somD, NomSom somA, int cout)
{
    int nMax = graphe->nMax;
    int rd = rang (graphe, somD);
    int rg = rang (graphe, somA);
    graphe->element [rd*nMax+rg] = vrai;
    graphe->valeur  [rd*nMax+rg] = cout;
}

void ecrireGraphe (GrapheMat* graphe)
{
    if (grapheDetruit == vrai)
    {
        printf ("Le graphe a ete detruit ou n'est pas encore cree\n");
    }
    else
    {
        int nMax = graphe->nMax;
        for (int i=0; i<graphe->n; i++) printf ("%s ", graphe->nomS[i]);
        printf (";\n");
        for (int i=0; i<graphe->n; i++)
        {
            printf ("\n%s : ", graphe->nomS[i]);
            for (int j=0; j<graphe->n; j++)
            {
                if (graphe->element [i*nMax+j] == vrai)
                {
                    printf ("%s ", graphe->nomS[j]) ;
                    if (graphe->value)
                    {
                        printf (" (%3d) ", graphe->valeur [i*nMax+j] );
                    }
                }
            }
            printf (";");
        }
    }
}


static void ecrireEtape (Matrice a, Matrice p, int k, int ns, int nMax)
{
   // printf ("\n A implementer");
    printf ("Passage par le sommet num?ro %d\n", k);
    for (int i=0; i<ns; i++) {
            for (int j=0; j<ns; j++) {
                if (a [i*nMax+j]==INFINI) { printf (" %3s", "*"); }
                else { printf (" %3d", a [i*nMax+j]); } }
                printf ("%6s", " ");
                for (int j=0; j<ns; j++) {
                        printf ("%3d", p [i*nMax+j]); }
                printf ("\n");}
                printf ("\n");
}

// initialiser les matrices a et p a partir de graphe
static void initFloyd (GrapheMat* graphe, Matrice* a, Matrice* p)
{
    //printf ("\n A implementer");
    int nMax = graphe->nMax;
    *a= (int*) malloc(sizeof(int) * nMax * nMax);
    *p = (int*) malloc(sizeof(int) * nMax * nMax);

    for (int i = 0; i < graphe->n; i++) {
        for (int j = 0; j < graphe->n; j++) {

            if (i == j) {
                (*a)[i*nMax+j]=0;
                (*p)[i * nMax + j] = i;
            }
            else {
                (*a)[i*nMax+j]=graphe->valeur [i*nMax+j];
                (*p)[i * nMax + j] = i;
            }
        }
    }



}

void floyd (GrapheMat* graphe)
{
     //printf ("\n A implementer");
     int nMax = graphe->nMax;
    Matrice a, p;
    initFloyd(graphe, &a, &p);

    for (int k = 0; k < graphe->n; k++) {
        for (int i = 0; i < graphe->n; i++) {
            for (int j = 0; j < graphe->n; j++) {
                if (a[i * nMax + k] != INFINI && a[k * nMax + j] != INFINI) {
                    int new_dist = a[i * nMax + k] + a[k * nMax + j];
                    if (new_dist < a[i * nMax + j]) {
                        a[i * nMax + j] = new_dist;
                        p[i * nMax + j] = p[k * nMax + j];
                    }
                }
            }
        }
        ecrireEtape(a,p,k,graphe->n,nMax);
}}


static void profondeur (GrapheMat* graphe, int numSommet)
{
    int nMax = graphe->nMax;
    graphe->marque [numSommet] = vrai;
    printf ("%s\n", graphe->nomS [numSommet]);
    for (int i=0; i<graphe->n; i++)
    {
        if ( (graphe->element [numSommet*nMax+i] == vrai)
                && !graphe->marque [i] )
        {
            profondeur (graphe, i);
        }
    }
}


void parcoursProfond (GrapheMat* graphe)
{
    initMarque (graphe);
    for (int i=0; i<graphe->n; i++)
    {
        if (!graphe->marque [i]) profondeur (graphe, i);
    }
}


// FIN include

int menu () {

  printf ("\n\nGRAPHES avec matrices\n\n");

  printf ("0 - Fin du programme\n");
  printf ("1 - Creation a partir d'un fichier\n");
  printf ("\n");
  printf ("2 - Initialisation d'un graphe vide\n");
  printf ("3 - Ajout d'un sommet\n");
  printf ("4 - Ajout d'un arc\n");
  printf ("\n");
  printf ("5 - Liste des sommets et des arcs\n");
  printf ("6 - Destruction du graphe\n");
  printf ("7 - Parcours en profondeur d'un graphe\n");
  printf ("\n");
  printf ("8  - Floyd \n");
  printf ("\n");
  printf ("Votre choix ? ");
  int cod; scanf ("%d", &cod); getchar();
  printf ("\n");
  return cod;
}
int main () {
  Graphe* graphe;
  booleen fini = faux;
  while (!fini) {
    switch ( menu() ) {
    case 0:
      fini = vrai;
      break;
    case 1: {  // cr�ation � partir d'un fichier
      printf ("Nom du fichier contenant le graphe ? ");
      char nomFe [50];
      scanf  ("%s", nomFe);
      //strcpy (nomFe, "graphe.txt");
      FILE* fe = fopen (nomFe, "r");
      if (fe == NULL) {
        perror (nomFe);
      } else {
        graphe = lireGraphe (fe, 20); // 20 sommets maximum
        fclose (fe);
      }
      } break;
   case 2: {  // cr�ation d�un graphe vide
      printf ("Nombre maximum de sommets ? ");
      int nMaxSom; scanf  ("%d", &nMaxSom);
      printf ("0) non valu�; 1) graphe valu� ? ");
      int value; scanf ("%d", &value);

      graphe = creerGrapheMat (nMaxSom, value);

    } break;
    case 3: {  // ajouter un sommet
      printf ("Nom du sommet � ins�rer ? ");
      NomSom somD; scanf  ("%s", somD);
      ajouterUnSommet (graphe, somD);
    } break;
    case 4: {  // ajouter un arc
      printf ("Nom du sommet de d�part ? ");
      NomSom somD; scanf  ("%s", somD);
      printf ("Nom du sommet d'arriv�e ? ");
      NomSom somA; scanf  ("%s", somA);
      int cout;
      if (graphe->value) {
        printf ("Cout de la relation ? ");
        scanf ("%d", &cout);
      } else {
        cout = 0;
      }
      ajouterUnArc (graphe, somD, somA, cout);
    } break;
    case 5:
      ecrireGraphe (graphe);
      break;
    case 6:
      detruireGraphe (graphe);
      break;
    case 7:
      parcoursProfond (graphe);
      break;
    case 8:
     if (graphe->value) {
        printf ("\nLes plus courts chemins\n\n");
          floyd (graphe);
      } else {
        printf ("Graphe non valu�\n");
      }
      break;
    }   // switch
    if (!fini) {
      printf ("\n\nTaper Return pour continuer\n");
      getchar();
    }
  }

return 0;
}
