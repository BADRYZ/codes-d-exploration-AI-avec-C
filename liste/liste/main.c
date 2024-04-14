#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"
#include "personne.h"
//INCLUDE

// Fonctions locales au module
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
Personne* creerPersonne (char* nom, char* prenom) {
  Personne* p = (Personne*)malloc(sizeof(Personne));
  strcpy (p->nom, nom);
  strcpy (p->prenom, prenom);
  return p;
}

char*  ecrirePersonne (Objet* objet) {
    Personne* p = (Personne*) objet;
    char* output      = (char*) malloc (sizeof (Personne));
    snprintf(output, sizeof (Personne), "%s %s\n", p->nom, p->prenom);
    return output;
}

int comparerPersonne (Objet* objet1, Objet* objet2) {
  Personne* p1 = (Personne*)objet1;
  Personne* p2 = (Personne*)objet2;
  return strcmp (p1->nom, p2->nom);
}

// FIN INCLUDE
int menu () {
  printf ("\n\nGESTION D'UNE LISTE DE PERSONNES\n\n");
  printf ("0 - Fin\n");
  printf ("1 - Insertion   en t�te de liste\n");
  printf ("2 - Insertion   en fin  de liste\n");
  printf ("3 - Retrait     en t�te de liste\n");
  printf ("4 - Retrait     en fin  de liste\n");
  printf ("5 - Retrait     d'un �l�ment � partir de son nom\n");
  printf ("6 - Parcours    de la liste\n");
  printf ("7 - Recherche   d'un �l�ment � partir de son nom\n");
  printf ("8 - Destruction de la liste\n");
  printf ("\n");
  printf ("Votre choix ? ");
  int cod; scanf ("%d", &cod);
  printf ("\n");
  return cod;
}

int  main () {


  booleen fini = faux;
  Liste* lp = creerListe (0, ecrirePersonne, comparerPersonne);
  while (!fini) {
    switch (menu() ) {
    case 0:
      fini = vrai;
      break;
    case 1 : {
	  printf ("Nom de la personne � cr�er ? ");
	  ch15 nom; scanf ("%s", nom);
      printf ("Pr�nom de la personne � cr�er ? ");
      ch15 prenom; scanf ("%s", prenom);
      Personne* nouveau = creerPersonne(nom, prenom);

      insererEnTeteDeListe (lp, nouveau);
      } break;
    case 2 : {
	  printf ("Nom de la personne � cr�er ? ");
	  ch15 nom; scanf ("%s", nom);
      printf ("Pr�nom de la personne � cr�er ? ");
      ch15 prenom; scanf ("%s", prenom);
      Personne* nouveau = creerPersonne(nom, prenom);

      insererEnFinDeListe (lp, nouveau);
      } break;
    case 3 : {
      Personne* extrait = (Personne*) extraireEnTeteDeListe (lp);
      if (extrait != NULL) {
        printf ("�l�ment %s %s extrait en t�te de liste",
                extrait->nom, extrait->prenom);
      } else {
        printf ("Liste vide");
      }
      } break;
    case 4 : {
      Personne* extrait = (Personne*) extraireEnFinDeListe (lp);
      if (extrait != NULL) {
        printf ("�l�ment %s %s extrait en fin de liste",
                extrait->nom, extrait->prenom);
      } else {
        printf ("Liste vide");
      }
      } break;
    case 5 : {
      printf ("Nom de la personne � extraire ? ");
      ch15 nom; scanf ("%s", nom);
      Personne* cherche = creerPersonne (nom, "?");
      Personne* pp      = (Personne*) chercherUnObjet (lp, cherche);
      if (extraireUnObjet (lp, pp)) {
        printf ("�l�ment %s %s extrait de la liste", pp->nom, pp->prenom);
      }
      } break;
    case 6:
      listerListe (lp);
      break;
    case 7 : {
      printf ("Nom de la personne recherch�e ? ");
      ch15 nom; scanf ("%s", nom);
      Personne* cherche = creerPersonne (nom, "?");
      Personne* pp      = (Personne*) chercherUnObjet (lp, cherche);
      if (pp != NULL) {
        printf ("%s %s trouv�e dans la liste\n", pp->nom, pp->prenom);
      } else {
        printf ("%s inconnue dans la liste\n", nom);
      }
      } break;
    case 8:
      detruireListe (lp);
      break;
    }
  }
    return 0;
}


