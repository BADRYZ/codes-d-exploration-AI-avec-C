#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arbre.h"
#include "liste.h"

booleen arbreVide (Arbre* arbre)
{
    return arbre->racine == NULL;
}
*
Noeud* cNd (Objet* objet, Noeud* gauche, Noeud* droite)
{
    Noeud* nouveau     = new Noeud();
    nouveau->reference = objet;
    nouveau->gauche    = gauche;
    nouveau->droite    = droite;
    return nouveau;
}

Noeud* cNd (Objet* objet)
{
    return cNd (objet, NULL, NULL);
}

Noeud* CcNd (char* message, Noeud* gauche, Noeud* droite)
{
    return CcNd ( (Objet*) message, gauche, droite);
}
*/

Noeud* cF (Objet* objet)
{
    return cNd (objet, NULL, NULL);
}

Noeud* CcF (char* message)
{
    return CcF ( (Objet*) message);
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


void Dprefixe (Noeud* racine, char* (*afficher) (Objet*))
{
    if (racine != NULL)
    {
        printf ("%s ", afficher (racine->reference));
        Dprefixe (racine->gauche, afficher);
        Dprefixe (racine->droite, afficher);
    }
}

void prefixe (Arbre* arbre)
{
    Dprefixe (arbre->racine, arbre->afficher);
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

void infixe (Arbre* arbre)
{
    Dinfixe (arbre->racine, arbre->afficher);
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

static char* toChar (Objet* objet)
{
    return (char*) objet;
}
static int noeudExplore=0;
static void enLargeurDabord (Noeud* racine, char* (*afficher) (Objet*), char but[])
{
    Liste* li = creerListe();
    insererEnFinDeListe (li, racine);
    while (!listeVide (li) )
    {
        Noeud* extrait = (Noeud*) extraireEnTeteDeListe (li);
        noeudExplore++;
        if(strcmp((char*)extrait->reference,but)==0){
            printf("\n=> Le but est trouve: %s ",but);
            return;
        }
        printf("%s ",toString(extrait->reference));
        if (extrait->gauche != NULL)
            insererEnFinDeListe (li,extrait->gauche);
        if (extrait->droite != NULL)
            insererEnFinDeListe (li,extrait->droite);
    }
    printf("\n=> le but %s est introuvable ",but);
}
static int noeudDevelopee=0;
void parcoursEnLargeur(Arbre* arbre){
    char but[20];

    printf("\n=> entrez le but: ");
    scanf("%s",but);
    printf("=> le chemin du recherche: ");

    enLargeurDabord(arbre->racine,arbre->afficher,but);
    printf("\n=> Le nombre de noeuds Explores : %d",noeudExplore);
    noeudDevelopee=noeudExplore-1;
    noeudExplore=0;
    printf("\n=> Le nombre de noeuds Developees : %d",noeudDevelopee);
    noeudDevelopee=0;
}

//profondeur


//profendeur limite

