#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arbre.h"
#include "liste.h"


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
void Dpostfixe (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        Dpostfixe (racine->gauche, afficher);
        Dpostfixe (racine->droite, afficher);
        printf ("%s ", afficher (racine->reference));
    }
}

void postfixe (Arbre* arbre)
{
    Dpostfixe (arbre->racine, arbre->afficher);
}
void infixe (Arbre* arbre)
{
    Dinfixe (arbre->racine, arbre->afficher);
}




void Dinfixe (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        Dinfixe (racine->gauche, afficher);
        printf ("%s ", afficher (racine->reference));
        Dinfixe (racine->droite, afficher);
    }
}


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
    Arbre* arbre = (Arbre*) malloc(sizeof(Arbre));
    initArbre (arbre, racine, afficher, comparer);
    return arbre;
}


void Dprefixe (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        printf ("%s ", afficher (racine->reference));
        Dprefixe (racine->gauche, afficher);
        Dprefixe (racine->droite, afficher);
    }
}

void OOprefixe (Arbre* arbre)
{
    Dprefixe (arbre->racine, arbre->afficher);
}
Noeud* cNd (Objet* objet, Noeud* gauche, Noeud* droite)
{
    Noeud* nouveau = (Noeud*) malloc(sizeof(Noeud));
    nouveau->reference = objet;
    nouveau->gauche    = gauche;
    nouveau->droite    = droite;
    return nouveau;
}
Noeud* OcNd (Objet* objet)
{
    return cNd (objet, NULL, NULL);
}

Noeud* CcNd (char* message, Noeud* gauche, Noeud* droite)
{
    return CcNd ( (Objet*) message, gauche, droite);
}


Noeud* cF (Objet* objet)
{
    return cNd (objet, NULL, NULL);
}

Noeud* CcF (char* message)
{
    return CcF ( (Objet*) message);
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
  if (li->dernier == NULL) {li->dernier = nouveau;}
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
void initListe (Liste* li, int type, char* (*afficher) (Objet*),int (*comparer) (Objet*, Objet*)) {
  li->premier  = NULL;
  li->dernier  = NULL;
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
void ouvrirListe (Liste* li) {
  li->courant = li->premier;
}
static Element* elementCourant (Liste* li) {
  Element* ptc = li->courant;
  if (li->courant != NULL) {
    li->courant = li->courant->suivant;
  }
  return ptc;
}

Objet* objetCourant (Liste* li) {
  Element* ptc = elementCourant (li);
  return ptc==NULL ? NULL : ptc->reference;
}
booleen finListe (Liste* li) {
  return li->courant==NULL;
}

void listerListe (Liste* li) {
  ouvrirListe (li);
  while (!finListe (li)) {
    Objet* objet = objetCourant (li);
    printf ("%s ", li->afficher (objet));
  }
}
booleen  listeVide (Liste* li) {
  return li->nbElt == 0;
}
Objet* extraireEnTeteDeListe (Liste* li) {
  Element* extrait = li->premier;
  if (!listeVide(li)) {
    li->premier = li->premier->suivant;
    if (li->premier==NULL) {li->dernier=NULL;}
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
        if (extrait->gauche != NULL) {insererEnFinDeListe (li,
                    extrait->gauche);}
        if (extrait->droite != NULL) {insererEnFinDeListe (li,
                    extrait->droite);}
    }
}

void enLargeur (Arbre* arbre)
{
    EnLargeur (arbre->racine, arbre->afficher);
}
////////////////////
char* afficherString (Objet* objet)
{
    return (char*) objet;
}

int comparerString (Objet* objet1, Objet* objet2)
{
    return strcmp ((char*)objet1,(char*)objet2);
}


Arbre* creerArbreGene ()
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

  fprintf (stderr, "Votre choix ? ");
  int cod; scanf ("%d", &cod); getchar();
  printf ("\n");
  return cod;
}


int main () {
  Arbre* arbre ;
  booleen fini = faux;

  while (!fini) {
    switch (menu (arbre) ) {
    case 1:
      fini = vrai;
      break;
    case 2:
      printf ("Creation de l'arbre genealogique\n");
      arbre = creerArbreGene();
      break;
    case 3: {
      printf ("Parcours prefixe\n");
      OOprefixe (arbre);
      } break;
    case 4:
      printf ("Parcours infixe\n");
      infixe (arbre);
      break;
    case 5:
      printf ("Parcours postfixe\n");
      postfixe (arbre);
      break;
    case 6:
      printf ("Entez un nom\n");
      char nom[20]; scanf ("%s", nom);
      if (trouverNoeud(arbre, nom))
        printf ("le nom est dans l'arbre");
      else
        printf ("le nom n'est pas dans l'arbre");
      break;
    case 7:
      printf ("Parcours en largeur\n");
      enLargeur (arbre);
      break;
    case 8:
      printf ("La taille de l'arbre est : %d \n", taille (arbre));
      break;
    case 9:
      if (estFeuille(arbre->racine))
        printf ("Le noeud test� est une feuille\n");
      else
        printf ("Le noeud test� n'est pas une feuille\n");
      break;
      break;
    case 10:
      printf ("Le nombre des feuilles est : %d \n", nbFeuilles (arbre));
      break;
    case 11:
      listerFeuilles (arbre);
      break;
    case 12:
      printf ("La hauteur de l'arbre est : %d \n", hauteur (arbre));
      break;
    case 13: {
        Arbre* arbre2 = creerArbreGene(); // Déclaration de la variable à l'intérieur d'un bloc

        if (egaliteArbre(arbre, arbre2))
            printf("Les arbres testés sont égaux");
        else
            printf("Les arbres testés ne sont pas égaux");

        break;
    }  // switch
    if (!fini) {
      printf ("\n\nTaper Return pour continuer\n"); getchar();
    }
  } // while
  return 0;
}}
