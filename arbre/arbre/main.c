#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "arbre.h"
static bool trouve=false;
static int noeudExplore=0;
static int noeudDevelopee=0;
//
Noeud* creerNoeudAvecEnfants(Objet* objet, Noeud* gauche, Noeud* droite)
{
    Noeud* nouveau     = (Noeud*) malloc(sizeof(Noeud));
    nouveau->reference = objet;
    nouveau->gauche    = gauche;
    nouveau->droite    = droite;
    return nouveau;
}

Noeud* creerNoeudSeul(Objet* objet)
{
    return creerNoeudAvecEnfants(objet, NULL, NULL);
}

Noeud* creerFeuille(Objet* objet)
{
    return creerNoeudSeul(objet);
}

Noeud* creerFeuilleAvecMessage(char* message)
{
    return creerFeuille((Objet*)(message));
}

Noeud* creerNoeudAvecMessageEtEnfants(char* message, Noeud* gauche, Noeud* droite)
{
    return creerNoeudAvecEnfants((Objet*)(message), gauche, droite);
}

//

void initArbre (Arbre* arbre, Noeud* racine,
       char* (*afficher) (Objet*), int (*comparer) (Objet*, Objet*))
{
    arbre->racine   = racine;
    arbre->afficher = afficher;
    arbre->comparer = comparer;
}
Arbre* creerArbre (Noeud* racine, char* (*afficher) (Objet*),
                   int (*comparer) (Objet*, Objet*))
{
    Arbre* arbre = (Arbre*)malloc(sizeof(Arbre));
    initArbre (arbre, racine, afficher, comparer);
    return arbre;
}
void Pprefixe (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        printf ("%s ", afficher (racine->reference));
        Pprefixe (racine->gauche, afficher);
        Pprefixe (racine->droite, afficher);
    }
}
void prefixe (Arbre* arbre)
{
    Pprefixe (arbre->racine, arbre->afficher);
}


void Iinfixe (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        Iinfixe (racine->gauche, afficher);
        printf ("%s ", afficher (racine->reference));
        Iinfixe (racine->droite, afficher);
    }
}

void infixe (Arbre* arbre)
{
    Iinfixe (arbre->racine, arbre->afficher);
}
void Ppostfixe (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        Ppostfixe (racine->gauche, afficher);
        Ppostfixe (racine->droite, afficher);
        printf ("%s ", afficher (racine->reference));
    }
}

void postfixe (Arbre* arbre)
{
    Ppostfixe (arbre->racine, arbre->afficher);
}
Noeud* TtrouverNoeud (Noeud* racine, Objet* objet,
                            int (*comparer) (Objet*, Objet*))
{
    Noeud* pNom;
    if (racine == NULL)
    {
        pNom = NULL;
    }
    else if (comparer (racine->reference, objet) == 0)
    {
        pNom = racine;
    }
    else
    {
        pNom = TtrouverNoeud (racine->gauche, objet, comparer);
        if (pNom == NULL) pNom = TtrouverNoeud (racine->droite, objet,
                                                   comparer);
    }
    return pNom;
}

Noeud* trouverNoeud (Arbre* arbre, Objet* objet)
{
    return TtrouverNoeud (arbre->racine, objet, arbre->comparer);
}

// liste
typedef struct element {
  Objet*          reference;
  struct element* suivant;
} Element;
typedef struct {
  Element* premier;
  Element* dernier;
  Element* courant;
  int      nbElt;
  int      type;    // 0:simple, 1:croissant, 2:d�croissant
  char*    (*afficher) (Objet*);
  int      (*comparer) (Objet*, Objet*);
} Liste;

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
static Element* creerElement () {
  return (Element*) malloc (sizeof (Element));
  //return new Element();
}
void insererEnTeteDeListe (Liste* li, Objet* objet) {
  Element* nouveau   = creerElement();
  nouveau->reference = objet;
  nouveau->suivant   = li->premier;
  li->premier        = nouveau;
  if (li->dernier == NULL) li->dernier = nouveau;
  li->nbElt++;
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
void insererEnFinDeListe (Liste* li,  Objet* objet) {
  insererApres (li, li->dernier, objet);
}
//
booleen  listeVide (Liste* li) {
  return li->nbElt == 0;
}
Objet* extraireEnTeteDeListe (Liste* li) {
  Element* extrait = li->premier;
  if (!listeVide(li)) {
    li->premier = li->premier->suivant;
    if (li->premier==NULL) li->dernier=NULL;
    li->nbElt--;
  }
  return extrait != NULL ? extrait->reference : NULL;
}
void EnLargeur (Noeud* racine, char* (*afficher) (Objet*))
{
    Liste* li = creerListe(0, afficher, NULL);;
    insererEnFinDeListe (li, racine);
    while (!listeVide (li) )
    {
        Noeud* extrait = (Noeud*) extraireEnTeteDeListe (li);
        printf ("%s ", afficher (extrait->reference));
        if (extrait->gauche != NULL) insererEnFinDeListe (li,
                    extrait->gauche);
        if (extrait->droite != NULL) insererEnFinDeListe (li,
                    extrait->droite);
    }
}

void enLargeur (Arbre* arbre)
{
    EnLargeur (arbre->racine, arbre->afficher);
}

int Taille (Noeud* racine)
{
    if (racine == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + Taille (racine->gauche) + Taille (racine->droite);
    }
}

int taille (Arbre* arbre)
{
    return Taille (arbre->racine);
}
booleen estFeuille (Noeud* racine)
{
    return (racine->gauche==NULL) && (racine->droite==NULL);
}
int NbFeuilles (Noeud* racine)
{
    if (racine == NULL)
    {
        return 0;
    }
    else if ( estFeuille (racine) )
    {
        return 1;
    }
    else
    {
        return NbFeuilles (racine->gauche) + NbFeuilles (racine->droite);
    }
}

int nbFeuilles (Arbre* arbre)
{
    return NbFeuilles (arbre->racine);
}
void ListerFeuilles (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        if (estFeuille (racine))
        {
            printf ("%s ", afficher (racine->reference));
        }
        else
        {
            ListerFeuilles (racine->gauche, afficher);
            ListerFeuilles (racine->droite, afficher);
        }
    }
}

void listerFeuilles (Arbre* arbre)
{
    ListerFeuilles (arbre->racine, arbre->afficher);
}
int max (int a, int b)
{
    return (a<b)?b:a;
}

int Hauteur (Noeud* racine)
{
    if (racine == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + max (Hauteur (racine->gauche),
                        Hauteur (racine->droite) );
    }
}

int hauteur (Arbre* arbre)
{
    return Hauteur (arbre->racine);
}

booleen EgaliteArbre (Noeud* racine1, Noeud* racine2,
                             int (*comparer) (Objet*, Objet*))
{
    booleen resu = faux;
    if ( (racine1==NULL) && (racine2==NULL) )
    {
        resu = vrai;
    }
    else if ( (racine1!=NULL) && (racine2!=NULL) )
    {
        if (comparer (racine1->reference, racine2->reference) == 0)
        {
            if (EgaliteArbre (racine1->gauche, racine2->gauche, comparer) )
            {
                resu = EgaliteArbre (racine1->droite, racine2->droite, comparer);
            }
        }
    }
    return resu;
}



booleen egaliteArbre (Arbre* arbre1, Arbre* arbre2)
{
    return EgaliteArbre (arbre1->racine, arbre2->racine, arbre1->comparer);
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


//TP1
static void EnProfondeurLimitee(Noeud* racine,char* (*afficher)(Objet*),char but[],int limite){
Liste* li=creerListe(1,NULL,NULL);
insererEnTeteDeListe(li,racine);
if(!trouve){
    if(racine!=NULL){
        Noeud* extrait=(Noeud* ) extraireEnFinDeListe(li);
        noeudExplore++;
        if(strcmp((char*)extrait->reference,but)==0){
            printf("\n => le but  est trouve:%S",but);
            trouve=true;

        }else if(limite==0){
            printf("%s",afficher(extrait->reference));
        }
        else{
            if(limite>0){
                printf("%s",afficher(extrait->reference));
                EnProfondeurLimitee(extrait->gauche,afficher,but,limite-1);
                EnProfondeurLimitee(extrait->droite,afficher,but,limite-1);
            }
        }

    }
}


}


void parcoursEnProfondeurLimite(Arbre* arbre){
char but[20];
int limite=0;
printf("\n => entrer le but :");
scanf("%s",but);
printf("\n => entrer la limite :");
scanf("%d",&limite);
printf("\n => le chemin du recherche  :");
EnProfondeurLimitee(arbre->racine,arbre->afficher,but,limite);
if(!trouve){
    trouve=false;
    printf("\n le but %s est introuvable",but);
}
printf("\n le nombre de noeuds explores :%d",noeudExplore);
noeudDevelopee=noeudExplore-1;
printf("\n le nombre de noeuds developees :%d",noeudDevelopee);
noeudDevelopee=0;
noeudExplore=0;
trouve=false;

}

//iterative en prof
void parcoursEnProfondeurIterative(Arbre* arbre){
    char but[20];
    int limite=0;
    printf("\n => entrer le but :");
    scanf("%s",but);
    printf("\n => entrer la limite :");
    scanf("%d",&limite);
    printf("\n => le chemin du recherche  :");
    for(int i=0;i<limite;i++){
        EnProfondeurLimitee(arbre->racine,arbre->afficher,but,i);
    }
    if(!trouve){
        trouve=false;
        printf("\n => le but %s est introuvable",but);
    }
    printf("\n le nombre de noeuds explores :%d",noeudExplore);
    noeudDevelopee=noeudExplore-1;
    printf("\n le nombre de noeuds developees :%d",noeudDevelopee);
    noeudDevelopee=0;
    noeudExplore=0;
    trouve=false;
}




//finn

char* afficherString (Objet* objet)
{
    return (char*) objet;
}

int comparerString (Objet* objet1, Objet* objet2)
{
    return strcmp ((char*)objet1,(char*)objet2);
}


/*Arbre* creerArbreGene ()
{
    Noeud* racine =
        cNd ( "Samir",
              cNd ( "Kamal",
                    cNd ( "Yassine",
                          NULL,
                          cNd ( "Hind", NULL, cF ("Yasmine") )
                        ),
                    cNd ( "Sarah", cF ("Karim"), NULL)
                  ),
              NULL
            );
    return creerArbre (racine, afficherString, comparerString);
}*/
Arbre* creerArbreGene()
{
    Noeud* racine =
        creerNoeudAvecMessageEtEnfants("Samir",
            creerNoeudAvecMessageEtEnfants("Kamal",
                creerNoeudAvecMessageEtEnfants("Yassine",
                    NULL,
                    creerNoeudAvecMessageEtEnfants("Hind", NULL, creerFeuilleAvecMessage("Yasmine"))
                ),
                creerNoeudAvecMessageEtEnfants("Sarah", creerFeuilleAvecMessage("Karim"), NULL)
            ),
            NULL
        );
    return creerArbre(racine, afficherString, comparerString);
}



int menu (Arbre* arbre) {
  printf ("\n\n ARBRES BINAIRES \n\n");
  printf (" 1 - Fin du programme\n");
  printf ("\n");
  printf (" 2 - Creation de l'arbre genealogique\n");
  printf ("\n");
  printf (" 3 - Parcours prefixe\n");
  printf (" 4 - Parcours infixe\n");
  printf (" 5 - Parcours postfixe\n");

  printf (" 6 - Trouver un Noeud\n");
  printf (" 7 - Parcours en largeur\n");
  printf (" 8 - Taille de l'arbre\n");
  printf (" 9 - estfeuille\n");
  printf ("10 - nombre des feuilles\n");
  printf ("11 - liste des feuilles\n");
  printf ("12 - Hauteur de l'arbre \n");
  printf ("13 - egalite entre deux arbres\n");
  printf ("\n");

  printf ("14 - parcours en profendeur limitee\n");
  printf ("15 - parcours en profendeur iterative \n");



  fprintf (stderr, "Votre choix ? ");
  int cod; scanf ("%d", &cod); getchar();
  printf ("\n");
  return cod;
}


int main() {
    Arbre* arbre = NULL;
    booleen fini = faux;

    while (!fini) {
        switch (menu(arbre)) {
            case 1:
                fini = vrai;
                break;
            case 2:
                printf("Creation de l'arbre genealogique\n");
                arbre = creerArbreGene();
                printf("creer ArbreGene bien\n");
                break;
            case 3:
                printf("Parcours prefixe\n");
                prefixe(arbre);
                break;
            case 4:
                printf("Parcours infixe\n");
                infixe(arbre);
                break;
            case 5:
                printf("Parcours postfixe\n");
                postfixe(arbre);
                break;
            case 6: {
                printf("Entez un nom\n");
                char nom[20];
                scanf("%s", nom);
                if (trouverNoeud(arbre, nom))
                    printf("le nom est dans l'arbre");
                else
                    printf("le nom n'est pas dans l'arbre");
                break;
            }
            case 7:
                printf("Parcours en largeur\n");
                enLargeur(arbre);
                break;
            case 8:
                printf("La taille de l'arbre est : %d \n", taille(arbre));
                break;
            case 9:
                if (estFeuille(arbre))
                    printf("Le noeud testé est une feuille\n");
                else
                    printf("Le noeud testé n'est pas une feuille\n");
                break;
            case 10:
                printf("Le nombre des feuilles est : %d \n", nbFeuilles(arbre));
                break;
            case 11:
                listerFeuilles(arbre);
                break;
            case 12:
                printf("La hauteur de l'arbre est : %d \n", hauteur(arbre));
                break;
            case 13: {
                Arbre* arbre2 = creerArbreGene();

                if (egaliteArbre(arbre, arbre2))
                    printf("Les arbres testés sont égaux");
                else
                    printf("Les arbres testés ne sont pas égaux");

                break;
            }
            case 14:
                printf("Parcours en profendeur limitee\n");
                parcoursEnProfondeurLimite(arbre);
                break;

            case 15:
                printf("Parcours en profendeur iterative\n");
                parcoursEnProfondeurIterative(arbre);
                break;


        }

        if (!fini) {
            printf("\n\nTaper Return pour continuer\n");
            getchar();
        }
    }

    return 0;
}
