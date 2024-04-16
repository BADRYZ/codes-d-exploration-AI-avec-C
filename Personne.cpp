#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personne.h"

Personne* creerPersonne (char* nom, char* prenom) {
  Personne* p = new Personne();
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
