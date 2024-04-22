#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "graphemat.h"
#include "liregraphe.h"
#include "liste.h"
#define MAX_PATH_LENGTH 100
static bool trouve = false;
static int noeudsVisite = 0;
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
static void razMarque(GrapheMat* graphe){
    for(int i=0;i<graphe->n;i++){
        graphe->marque[i]=faux;
    }
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
    razMarque (graphe);
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
static void profondeur(GrapheMat *graphe, int numSommet, char but[]) {

    if (!trouve) {
        int nMax = graphe->nMax;
        graphe->marque[numSommet] = vrai;
        printf("->%s", graphe->nomS[numSommet]);
        noeudsVisite++;
        for (int i = 0; i < graphe->n; i++) {
            if ((graphe->element[numSommet * nMax + i] == vrai)
                && !graphe->marque[i] && !trouve) {
                if (strcmp(graphe->nomS[i], but) == 0) {

                    printf("->%s (Noeud but) ", but);
                    trouve = true;
                }
                profondeur(graphe, i, but);
            }
        }
    }
}
void parcoursProfond(GrapheMat *graphe) {

    razMarque(graphe);
    char str[20];
    printf("Tapez le sommet objectif(but) :");
    scanf("%s", str);
    printf("Chemin suivi par le parcous :   ");



    for (int i = 0; i < graphe->n; i++) {
        if (!graphe->marque[i])
            profondeur(graphe, i, str);
    }
    if (!trouve) {

        printf("\nLe noeud %s est INTROUVABLE", str);
    }

    printf("\nLa liste des neoueds visites : %d", noeudsVisite);
    noeudsVisite = 0;
}
/*

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
}*/


// FIN include

//***************include liste

static Element* creerElement    ();
static void     insererApres    (Liste* li, Element* precedent, Objet* objet);
static Objet*   extraireApres   (Liste* li, Element* precedent);
static Element* elementCourant  (Liste* li);




void initListe (Liste* li, int type, char* (*afficher) (Objet*),int (*comparer) (Objet*, Objet*)) {
  li->premier  = NULL;
  li->dernier  = NULL;
  li->courant  = NULL;
  li->nbElt    = 0;
  li->type     = type;
  li->afficher = afficher;
  li->comparer = comparer;
}

Liste* creerListe (int type, char* (*afficher) (Objet*), int (*comparer) (Objet*, Objet*)) {
  Liste* li = (Liste*) malloc (sizeof (Liste));
  initListe (li, type, afficher, comparer);
  return li;
}

booleen  listeVide (Liste* li) {
  return li->nbElt == 0;
}

int nbElement (Liste* li) {
  return li->nbElt;
}

// INSERER UN OBJET DANS UNE LISTE

void insererEnTeteDeListe (Liste* li, Objet* objet) {
  Element* nouveau   = creerElement();
  nouveau->reference = objet;
  nouveau->suivant   = li->premier;
  li->premier        = nouveau;
  if (li->dernier == NULL) li->dernier = nouveau;
  li->nbElt++;
}

void insererEnFinDeListe (Liste* li,  Objet* objet) {
  insererApres (li, li->dernier, objet);
}

// PARCOURS DE LISTE

void ouvrirListe (Liste* li) {
  li->courant = li->premier;
}

booleen finListe (Liste* li) {
  return li->courant==NULL;
}

Objet* objetCourant (Liste* li) {
  Element* ptc = elementCourant (li);
  return ptc==NULL ? NULL : ptc->reference;
}

void listerListe (Liste* li) {
  ouvrirListe (li);
  while (!finListe (li)) {
    Objet* objet = objetCourant (li);
    printf ("%s ", li->afficher (objet));
  }
}

Objet* chercherUnObjet (Liste* li, Objet* objetCherche) {
  booleen trouve = faux;
  Objet* objet;       // pointeur courant
  ouvrirListe (li);
  while (!finListe (li) && !trouve) {
    objet  = objetCourant (li);
    trouve = li->comparer (objetCherche, objet) == 0;
  }
  return trouve ? objet : NULL;
}


// EXTRAIRE UN OBJET D'UNE LISTE

Objet* extraireEnTeteDeListe (Liste* li) {
  Element* extrait = li->premier;
  if (!listeVide(li)) {
    li->premier = li->premier->suivant;
    if (li->premier==NULL) li->dernier=NULL;
    li->nbElt--;
  }
  return extrait != NULL ? extrait->reference : NULL;
}

Objet* extraireEnFinDeListe (Liste* li) {
  Objet* extrait;
  if (listeVide(li)) {
    extrait = NULL;
  } else if (li->premier == li->dernier) {
    extrait = extraireEnTeteDeListe (li);
  } else {
    Element* ptc = li->premier;
    while (ptc->suivant != li->dernier) ptc = ptc->suivant;
    extrait = extraireApres (li, ptc);
  }
  return extrait;
}

booleen extraireUnObjet (Liste* li, Objet* objet) {
  Element* precedent = NULL;
  Element* ptc       = NULL;
  booleen trouve = faux;

  ouvrirListe (li);
  while (!finListe (li) && !trouve) {
    precedent = ptc;
    ptc       = elementCourant (li);
    trouve = (ptc->reference == objet) ? vrai : faux;
  }
  if (!trouve) return faux;

  Objet* extrait = extraireApres (li, precedent);
  return vrai;
}

void detruireListe (Liste* li) {
  ouvrirListe (li);
  while (!finListe (li)) {
    Element* ptc = elementCourant (li);
    //free (ptc->reference);  // si on veut d�truire les objets de la liste
    free (ptc);
  }
  initListe (li, 0, NULL, NULL);
}

// LISTE ORDONNEE

// objet1 et objet2 sont-ils en ordre ?
static booleen enOrdre (Objet* objet1, Objet* objet2, booleen ordreCroissant,
                 int (*comparer) (Objet*, Objet*)) {
  booleen ordre = comparer (objet1, objet2) < 0;
  if (!ordreCroissant) ordre = !ordre;
  return ordre;
}

void insererEnOrdre (Liste* li, Objet* objet) {
  if (listeVide (li) ) {   // liste vide
    insererEnTeteDeListe (li, objet);
  } else {
    Element* ptc = li->premier;
    if ( enOrdre (objet, ptc->reference, li->type==1, li->comparer) ) {
      // insertion avant le premier �l�ment
      insererEnTeteDeListe (li, objet);
    } else {
	  // insertion en milieu ou fin de liste
      booleen  trouve = faux;
      Element* prec   = NULL;
      while (ptc != NULL && !trouve) {
        prec   = ptc;
        ptc    = ptc->suivant;
        if (ptc!=NULL) trouve = enOrdre (objet, ptc->reference, li->type==1, li->comparer);
      }
      insererApres (li, prec, objet);
    }
  }
}

// FONCTIONS LOCALES

static Element* creerElement () {
  return (Element*) malloc (sizeof (Element));
  //return new Element();
}

static void insererApres (Liste* li, Element* precedent, Objet* objet) {
  if (precedent == NULL) {
    insererEnTeteDeListe (li, objet);
  } else {
    Element* nouveau   = creerElement();
    nouveau->reference = objet;
    nouveau->suivant   = precedent->suivant;
    precedent->suivant = nouveau;
    if (precedent == li->dernier) li->dernier = nouveau;
    li->nbElt++;
  }
}

static Objet* extraireApres (Liste* li, Element* precedent) {
  if (precedent == NULL) {
    return extraireEnTeteDeListe (li);
  } else {
    Element* extrait = precedent->suivant;
    if (extrait != NULL) {
      precedent->suivant = extrait->suivant;
      if (extrait == li->dernier) li->dernier = precedent;
      li->nbElt--;
    }
    return extrait != NULL ? extrait->reference : NULL;
  }
}


static Element* elementCourant (Liste* li) {
  Element* ptc = li->courant;
  if (li->courant != NULL) {
    li->courant = li->courant->suivant;
  }
  return ptc;
}

// ********fin include liste

// *** include TP1

static int largeurD(GrapheMat* graphe, Liste* li, int numSommet,char but[]){
int nMax =graphe->nMax;
NomSom* extraite=NULL;
insererEnFinDeListe(li,graphe->nomS[numSommet]);
graphe->marque[numSommet]=true;
while(!listeVide(li)){
    extraite=(NomSom*)extraireEnTeteDeListe(li);
    noeudsVisite++;
    if(strcmp(*extraite,but)==0){
        printf("\n=> le but est trouver:%s:",but);
        return 1;


    }
    numSommet=rang(graphe, *extraite);
    printf("\n %s",extraite);
    for(int i=0;i<graphe->nMax;i++){
        if((graphe->element[numSommet*nMax+i]==vrai)&& !graphe->marque[i]){
            insererEnFinDeListe(li,graphe->nomS[i]);
            graphe->marque[i]=vrai;
        }
    }
    if(numSommet==graphe->n){
        printf("\n => le but %s est introuvable!",but);
        return 0;

    }
}
}

void parcoursLargeurD(GrapheMat* graphe){
    razMarque(graphe);
    Liste* li=creerListe(0, NULL, NULL);
    char str[20];
    printf("=> entrer le but");
    scanf("%s",str);
    printf("=> chemin de la recherche");
    int trouve=0;
    for(int i =0 ;i<graphe->n;i++){
        if(!graphe->marque[i]){
            trouve=largeurD(graphe,li,i,str);

        }
        if(trouve==1){
            break;
        }
    }
    printf("\n => le nombre de noeuds visites :%d",noeudsVisite);
    noeudsVisite=0;
}

// profendeur limite
static void profendeurLimite(GrapheMat* graphe ,int k,char but[],int limite){

if(!trouve){
    noeudsVisite++;
    int nMax=graphe->nMax;
    if(strcasecmp(graphe->nomS[k],but)==0){
        printf("%s",graphe->nomS[k]);
        trouve=vrai;
        return;
    }
    else if(limite<=0) {
        printf("%s",graphe->nomS[k]);
        graphe->marque[k]=vrai;
        return;
    }
    else {
        if(limite>0){
            printf("%s",graphe->nomS[k]);
            graphe->marque[k]=vrai;
            for(int i=0;i>graphe->n;i++){
                if((graphe->element[k*nMax+i]==vrai)&&!graphe->marque[i]&&!trouve){
                    profendeurLimite(graphe,i,but,limite-1);
                }
            }
        }
    }
}
}
void parcoursProfendeurLimite(GrapheMat* graphe){
razMarque(graphe);
Liste* li=creerListe(0, NULL, NULL);
char str[20];
printf("=> entre le but ");
scanf("%s",str);
int limite;
printf("=> entre la limite ");
scanf("%s",&limite);
for(int i=0;i<graphe->n;i++){
    if(!graphe->marque[i]){
        profendeurLimite(graphe,i,str,limite);
        break;
    }

}
if (trouve==faux){
    printf("\n le but est introuvable\n");

}
else {
    printf("\n => le nombre de noeuds visites :%d",noeudsVisite);
}
noeudsVisite=0;
trouve=faux;
}
//parcours iterative en pr
void profondeurIteretif(GrapheMat* graphe,int numSommet,int numNiveau,char but[]){
if(!trouve){
    noeudsVisite++;
    if(strcmp(graphe->nomS[numSommet],but)==0){
        printf("\n => le but est trouve!:%s",but);
        trouve=vrai;
    }
    else if (numNiveau==0){
        printf("-> %s ",graphe->nomS[numSommet]);
        graphe->marque[numSommet]=vrai;

    }
    else{
        if(numNiveau>0){
            int nMax=graphe->nMax;
            graphe->marque[numSommet]=vrai;
            printf(" -> %s ",graphe->nomS[numSommet]);
            for(int i=0;i<graphe->n;i++){
                if((graphe->element[numSommet*nMax+i]== vrai)&& !graphe->marque[i]){
                    profondeurIteretif(graphe,i,numNiveau-1,but);
                }
            }
        }
    }
}


}
void parcoursIterativeEnprofendeur(GrapheMat* graphe){
    char str[20];
    int numNiveau;
    int niveau;
    printf("entrer le but");
    scanf("%s",str);
    printf("entrer le max de niveau");
    scanf("%d",&niveau);
    razMarque(graphe);
    Liste* li=creerListe(0,NULL,NULL);
    printf("chemin suivi par le parcours:");
    for(numNiveau=0;numNiveau<=niveau;numNiveau++){
        if(trouve){
            break;
        }
        printf("limite %d",numNiveau);
        razMarque(graphe);
        for(int i=0;i<graphe->n;i++){
            if(!graphe->marque[i]){
                profondeurIteretif(graphe,i,numNiveau,str);
                break;
            }
        }
        printf("\n");
    }
    if(!trouve){
        trouve=false;
        printf("\n le noeud %s est introuvable",str);
    }
    printf("\nouds visites :%d",noeudsVisite);
    noeudsVisite=0;
    trouve=false;

}

//**** fin include TP1

//cout uniforme
void DinsererEnTeteDeListe(Liste* li, Objet* objet, Objet* priority)
{
    Element* nouveau = creerElement();

    nouveau->reference = objet;
    nouveau->priority = priority;
    nouveau->suivant = li->premier;
    li->premier = nouveau;
    if (li->dernier == NULL)
        li->dernier = nouveau;
    li->nbElt++;
}
static void DinsererApres(Liste* li, Element* precedent, Objet* objet, Objet* priority)
{
    if (precedent == NULL) {
        DinsererEnTeteDeListe(li, objet, priority);
    }
    else {
        Element* nouveau = creerElement();
        nouveau->reference = objet;

        nouveau->priority = priority;
        nouveau->suivant = precedent->suivant;
        precedent->suivant = nouveau;
        if (precedent == li->dernier)
            li->dernier = nouveau;
        li->nbElt++;
    }
}
void DinsererEnFinDeListe(Liste* li, Objet* objet, Objet* priority)
{
    DinsererApres(li, li->dernier, objet, priority);
}
void DinsererEnOrdre(Liste* li, Objet* objet, Objet* priority)
{
    if (listeVide(li)) { // liste vide
        DinsererEnTeteDeListe(li, objet, priority);
        //printf ("insertion dans liste vide\n");
    }
    else {
        Element* ptc = li->premier;
        if (enOrdre(priority, ptc->priority, li->type == 1, li->comparer)) {
            // insertion avant le premier élément
            //printf ("insertion en tête de liste non vide\n");
            DinsererEnTeteDeListe(li, objet, priority);
        }
        else { // insertion en milieu ou fin de liste
            //printf ("insertion en milieu ou fin de liste non vide\n");
            booleen trouve = faux;
            Element* prec = NULL;
            while (ptc != NULL && !trouve) {
                prec = ptc;
                ptc = ptc->suivant;
                if (ptc != NULL)
                    trouve = enOrdre(priority, ptc->priority, li->type == 1, li->comparer);
            }
            // insertion en milieu de liste ou fin de liste
            DinsererApres(li, prec, objet, priority);
        }
    }
}
bool chercherUnObjetBis(Liste* li, Objet* objetCherche)
{
    bool trouve = faux;
    Objet* objet; // pointeur courant
    ouvrirListe(li);
    while (!finListe(li) && !trouve) {
        objet = objetCourant(li);
        trouve = li->comparer(objetCherche, objet) == 0;
    }
    return trouve;
}
static int comparerCar(Objet* objet1, Objet* objet2)
{
    return strcmp((char*)objet1, (char*)objet2);
}

static char* toChar(Objet* objet)
{
    return (char*)objet;
}

Liste* DcreerListe(int type)
{
    return creerListe(type, toChar, comparerCar);
}


//typedef char NomS[50];
static NomSom *path = NULL;
static int *cout = NULL;

void initialiserTableaux() {
    path = (NomSom *) malloc(sizeof(NomSom) * 100);
    cout = (int *) malloc(sizeof(int) * 100);
    if (path == NULL || cout == NULL) {
        printf("erreur du path et du cout ");
        exit(EXIT_FAILURE);
    }
}

void libererTableaux() {
    free(path);
    free(cout);
}
static void coutUniforme(GrapheMat *graphe, Liste *li, int numSommet, int but) {

    for (int j = 0; j < 100; j++) {
        strcpy(*(path + j), "");
    }

    strcpy(*(path + numSommet), graphe->nomS[numSommet]);
    int nMax = graphe->nMax;
    Element *extraite = NULL;

    DinsererEnFinDeListe(li, graphe->nomS[numSommet], cout + numSommet);
    graphe->marque[numSommet] = true;

    while (!listeVide(li) && !trouve) {

        extraite = (Element *) extraireEnTeteDeListe(li);
        noeudsVisite++;
        numSommet = rang(graphe, (char *) extraite);

        if (numSommet == but) {
            trouve = true;
            return;
        }

        for (int i = 0; i < graphe->n; i++) {

            if ((graphe->element[numSommet * nMax + i] == vrai)
                && !graphe->marque[i]) {

                strcat(*(path + i), *(path + numSommet));
                strcat(*(path + i), "->");
                strcat(*(path + i), graphe->nomS[i]);

                *(cout + i) = graphe->valeur[numSommet * nMax + i] + *(cout + numSommet);

                DinsererEnOrdre(li, graphe->nomS[i], cout + i);

                graphe->marque[i] = vrai;
            } else {
                bool trouvee = chercherUnObjetBis(li, graphe->nomS[i]);
                if ((graphe->element[numSommet * nMax + i] == vrai) && trouvee &&
                    *(cout + i) > graphe->valeur[numSommet * nMax + i] + *(cout + numSommet)) {
                    *(cout + i) = graphe->valeur[numSommet * nMax + i] + *(cout + numSommet);
                    strcpy(*(path + i), "");
                    strcat(*(path + i), *(path + numSommet));
                    strcat(*(path + i), "->");
                    strcat(*(path + i), graphe->nomS[i]);
                }
            }
        }
    }
}

void parcoursCoutUniforme(GrapheMat *graphe) {


    int but;
    printf("Entrez le numero du sommet but :");
    scanf("%d", &but);
    Liste *li = DcreerListe(1);
    razMarque(graphe);
    for (int j = 0; j < graphe->n; j++) {
        *(cout + j) = 0;
    }

    for (int i = 0; i < graphe->n; i++) {
        if (!graphe->marque[i]) {
            *(cout + i) = 0;
            coutUniforme(graphe, li, i, but);
        }

        break;
    }

    if (trouve) {
        printf("Le plus court  chemin vers le noeud   %s   :", graphe->nomS[but]);
        printf("%s\n", *(path + but));
        printf("Le cout de ce chemin :  %d ", *(cout + but));
        trouve = false;
    } else {
        printf("\nNoeud INTROUVABLE");
    }

    printf("\nNoeuds visites : %d", noeudsVisite);
    noeudsVisite = 0;
}

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
  printf (" ***** EXPLORATION ***** \n");
  printf ("\n");
  printf ("9   -  exploration en largeur d'abord \n");
  printf ("10  -  exploration en profendeur d'abord limitee\n");
  printf ("11  -  Exploration iterative en profondeur\n");
  printf ("12  -  Exploration cout uniforme\n");
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
      printf ("0) non value; 1) graphe value ? ");
      int value; scanf ("%d", &value);

      graphe = creerGrapheMat (nMaxSom, value);

    } break;
    case 3: {  // ajouter un sommet
      printf ("Nom du sommet a inserer ? ");
      NomSom somD; scanf  ("%s", somD);
      ajouterUnSommet (graphe, somD);
    } break;
    case 4: {  // ajouter un arc
      printf ("Nom du sommet de depart ? ");
      NomSom somD; scanf  ("%s", somD);
      printf ("Nom du sommet d'arrivee ? ");
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
        printf ("Graphe non value\n");
      }
      break;
    case 9:
        printf("exploration en largeur d'abord ");
        parcoursLargeurD(graphe);
        break;
    case 10:
        printf("exploration en profendeur d'abord limitee");
        parcoursProfendeurLimite(graphe);
        break;
    case 11:
        printf("Exploration iterative en profondeur");
        parcoursIterativeEnprofendeur(graphe);
        break;
    case 12:
        initialiserTableaux();
        printf("Exploration cout uniforme");
        parcoursCoutUniforme(graphe);
        libererTableaux();
        break;


    }   // switch
    if (!fini) {
      printf ("\n\nTaper Return pour continuer\n");
      getchar();
    }
  }

return 0;
}
