#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arbre.h"
#include "liste.h"

booleen arbreVide (Arbre* arbre)
{
    return arbre->racine == NULL;
}

Noeud* cNd (Objet* objet, Noeud* gauche, Noeud* droite)
{
    Noeud* nouveau     = (Noeud*)malloc(sizeof(Noeud));
    nouveau->reference = objet;
    nouveau->gauche    = gauche;
    nouveau->droite    = droite;
    return nouveau;
}

Noeud* cNd (Objet* objet)
{
    return cNd (objet, NULL, NULL);
}

Noeud* cF (Objet* objet)
{
    return cNd (objet, NULL, NULL);
}

Noeud* MessagecF (char* message)
{
    return cF ( (Objet*) message);
}

Noeud* messageCnd (char* message, Noeud* gauche, Noeud* droite)
{
    return cNd ( (Objet*) message, gauche, droite);
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
    Arbre* arbre = new Arbre();
    initArbre (arbre, racine, afficher, comparer);
    return arbre;
}


void prefixe (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        printf ("%s ", afficher (racine->reference));
        prefixe (racine->gauche, afficher);
        prefixe (racine->droite, afficher);
    }
}

void prefixe (Arbre* arbre)
{
    prefixe (arbre->racine, arbre->afficher);
}


void infixe (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        infixe (racine->gauche, afficher);
        printf ("%s ", afficher (racine->reference));
        infixe (racine->droite, afficher);
    }
}

void infixe (Arbre* arbre)
{
    infixe (arbre->racine, arbre->afficher);
}

void postfixe (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        postfixe (racine->gauche, afficher);
        postfixe (racine->droite, afficher);
        printf ("%s ", afficher (racine->reference));
    }
}

void postfixe (Arbre* arbre)
{
    postfixe (arbre->racine, arbre->afficher);
}


Noeud* trouverNoeud (Noeud* racine, Objet* objet,
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
        pNom = trouverNoeud (racine->gauche, objet, comparer);
        if (pNom == NULL) pNom = trouverNoeud (racine->droite, objet,
                                                   comparer);
    }
    return pNom;
}

Noeud* trouverNoeud (Arbre* arbre, Objet* objet)
{
    return trouverNoeud (arbre->racine, objet, arbre->comparer);
}

void enLargeur (Noeud* racine, char* (*afficher) (Objet*))
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
    enLargeur (arbre->racine, arbre->afficher);
}

int taille (Noeud* racine)
{
    if (racine == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + taille (racine->gauche) + taille (racine->droite);
    }
}

int taille (Arbre* arbre)
{
    return taille (arbre->racine);
}


booleen estFeuille (Noeud* racine)
{
    return (racine->gauche==NULL) && (racine->droite==NULL);
}

int nbFeuilles (Noeud* racine)
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
        return nbFeuilles (racine->gauche) + nbFeuilles (racine->droite);
    }
}

int nbFeuilles (Arbre* arbre)
{
    return nbFeuilles (arbre->racine);
}

void listerFeuilles (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        if (estFeuille (racine))
        {
            printf ("%s ", afficher (racine->reference));
        }
        else
        {
            listerFeuilles (racine->gauche, afficher);
            listerFeuilles (racine->droite, afficher);
        }
    }
}

void listerFeuilles (Arbre* arbre)
{
    listerFeuilles (arbre->racine, arbre->afficher);
}

int max (int a, int b)
{
    return (a<b)?b:a;
}

int hauteur (Noeud* racine)
{
    if (racine == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + max (hauteur (racine->gauche),
                        hauteur (racine->droite) );
    }
}

int hauteur (Arbre* arbre)
{
    return hauteur (arbre->racine);
}


booleen egaliteArbre (Noeud* racine1, Noeud* racine2,
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
            if (egaliteArbre (racine1->gauche, racine2->gauche, comparer) )
            {
                resu = egaliteArbre (racine1->droite, racine2->droite, comparer);
            }
        }
    }
    return resu;
}



booleen egaliteArbre (Arbre* arbre1, Arbre* arbre2)
{
    return egaliteArbre (arbre1->racine, arbre2->racine, arbre1->comparer);
}
