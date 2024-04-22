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
