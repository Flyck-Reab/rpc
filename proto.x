/* exo3.x: protocole de gestion de fichiers a distance */

/* longueur maximale d'un nom de fichier*/
const MAXNOM = 255;
/* longueur maximale d'un bloc de transfert*/
const MAXBLOC = 1024;

/* nom de fichier */
typedef string type_nom<MAXNOM>;
/* liste de noms de fichiers */
typedef struct cell_nom *liste_nom;
/* une cellule de la liste de fichiers */
struct cell_nom {
    type_nom  nom;        /* nom de fichier */
    liste_nom suivant;    /* autres noms de fichiers */
}; 

/* resultat de la lecture d'un repertoire */
union ls_res switch (int erreur) {
case 0:
    /* si pas d'erreur : renvoie de la liste */
    liste_nom liste;
default:
    /* si une erreur : on ne renvoie rien d'autre */
    void;
};


/* bloc de transfer */
typedef string type_bloc<MAXBLOC>;
/* liste de blocs */
typedef struct cell_bloc *liste_bloc;
/* une cellule de la liste de blocs */
struct cell_bloc {
    type_bloc  bloc;       /* un bloc */
    liste_bloc suivant;    /* blocs suivant */
}; 

/* resultat de la lecture d'un fichier */
union read_res switch (int erreur) {
case 0:
    /* si pas d'erreur : renvoie le fichier morcele */
    liste_bloc fichier;
default:
    /* si une erreur : on ne renvoie rien d'autre */
    void;
};

/* parametres de l'ecriture d'un fichier */
struct write_parm {
	type_nom   nom; /* nom du fichier */
	bool	   ecraser; /* si == 1 on ecrase sinon on concatene */
	liste_bloc donnees; /* donnees */
};

/* definition du programme de gestion de fichiers */
program PROTO_GESTION_FICHIER {
        version PROTO_GESTION_FICHIER_V1 {
            ls_res LS(type_nom) = 1;
	    read_res READ(type_nom) = 2;
	    int WRITE(write_parm) = 3;
        } = 1;
}=0x20200112;
