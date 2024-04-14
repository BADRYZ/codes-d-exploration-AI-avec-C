
// ignorer les blancs
void lireBlancs (FILE* fe);

// lire un nom de sommet en ignorant les espaces
void lireUnMot (FILE* fe, char* chaine);

// fournir un pointeur sur un graphe construit
// � partir d'un fichier fe de donn�es
GrapheMat* lireGraphe (FILE* fe, int nMaxSom);
