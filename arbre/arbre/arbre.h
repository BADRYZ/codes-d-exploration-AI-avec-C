typedef int  booleen;
#define faux 0
#define vrai 1

typedef void Objet;

typedef struct noeud
{
    Objet*        reference;
    struct noeud* gauche;
    struct noeud* droite;
} Noeud;

typedef struct
{
    Noeud* racine;
    char* (*afficher) (Objet*);
    int   (*comparer) (Objet*, Objet*);
} Arbre;


// cr�ation de noeuds
Noeud*     cNd	(Objet* objet, Noeud* Gauche, Noeud* Droite);
Noeud*     messageCnd  (char* message, Noeud* gauche, Noeud* droite);
Noeud*     cF   (Objet* objet);
Noeud*     MessagecF   (char* message);

// cr�ation d'arbre
void     initArbre (Arbre* arbre, Noeud* racine,  char* (*afficher) (Objet*), int (*comparer) (Objet*, Objet*));
Arbre*   creerArbre (Noeud* racine, char* (*afficher) (Objet*), int (*comparer) (Objet*, Objet*));

// parcours
void     prefixe              (Arbre* arbre);
void     infixe               (Arbre* arbre);
void     postfixe             (Arbre* arbre);
Noeud*   trouverNoeud         (Arbre* arbre, Objet* objet);
void     enLargeur            (Arbre* arbre);

int      taille               (Arbre* arbre);
booleen  estFeuille           (Noeud* arbre);
int      nbFeuilles           (Arbre* arbre);
void     listerFeuilles       (Arbre* arbre);
int      hauteur              (Arbre* arbre);
booleen  egaliteArbre         (Arbre* arbre1, Arbre* arbre2);
