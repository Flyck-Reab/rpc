/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "proto.h"
#include <unistd.h>
#include <dirent.h>

int connexionValide(struct svc_req *rqstp)
{
	switch(rqstp->rq_cred.oa_flavor)
	{
		case AUTH_UNIX:
			printf("Authentifié\n");
			struct authunix_parms* aup = (struct authunix_parms *) rqstp->rq_clntcred;
			printf("uid serveur : %d \nuid client : %d\n", getuid(), aup->aup_uid);
			printf("gid client : %d\n", aup->aup_gids);
			printf("hostname client : %s\n\n", aup->aup_machname);
			return 2;
		default :
			printf("Non authentifié\n");
			return 3;
	}
}


ls_res *
ls_1_svc(type_nom *argp, struct svc_req *rqstp)
{
	printf("-----------------Debut du programme ls---------------\n\n");
	static ls_res  result;
	ls_res resultatTmp;
	resultatTmp.erreur=1;

	resultatTmp.erreur=connexionValide(rqstp);
	if(resultatTmp.erreur<=2)
	{

		DIR *directory;
		struct dirent *infosReadDir;
		char* dossier = *argp;

		cell_nom *celluleCourante;
		cell_nom *celluleSuivante;

		//Ouverture du fichier
		// directory = opendir((char *)argp);
		directory = opendir(dossier);
		if(directory==NULL)
		{
			perror("Erreur d'ouverture du dossier ");
			printf("Dossier : \"%s\" \n",dossier);
			resultatTmp.erreur=4;
		}
		
		//Si le fichier est ouvert sans erreur
		else
		{
			//initialisation de cellule courante
			celluleCourante=malloc(10*sizeof(char));
			//initialisation de la cellule suivante
			celluleSuivante=celluleCourante;

			//initialisation de resultatTmp
			resultatTmp.ls_res_u.liste=celluleCourante;
		

			while ((infosReadDir = readdir(directory)) != NULL)
			{	
				celluleCourante->suivant=celluleSuivante;
				celluleCourante=celluleSuivante;

				//On prepare la cellule a recevoir les valeurs
				celluleCourante->nom=calloc(MAXNOM,sizeof(char));
				celluleCourante->suivant=NULL;		

				//On ecrit les valeurs
				strcpy(celluleCourante->nom, infosReadDir->d_name);

				//initialisation de la cellule suivante
				celluleSuivante=malloc(10*sizeof(char));

			}
			closedir(directory);	
			resultatTmp.erreur=0;
		}
	}
	result = resultatTmp;
	printf("\nErreur retournée (0 = OK) : %d\n", result.erreur);
	printf("\n\n------------------Fin du programme ls------------------\n\n");
	return &result;	
}

read_res *
read_1_svc(type_nom *argp, struct svc_req *rqstp)
{
	printf("----------------Debut du programme read----------------\n\n");
	static read_res  result;
	read_res resultatTmp;
	resultatTmp.erreur=1;
	
	resultatTmp.erreur=connexionValide(rqstp);
	if(resultatTmp.erreur<=2)
	{

		DIR *directory;
		FILE *file;
		struct dirent *infosReadDir;
		char* fichier = *argp;

		cell_bloc *celluleCourante;
		cell_bloc *celluleSuivante;
		char buffer[MAXBLOC];

		directory = opendir(fichier);
		if(directory!=NULL){
			closedir(directory);
			resultatTmp.erreur=4;
		}
		else
		{
			//Ouverture du fichier
			file = fopen(fichier,"r");
			if(file==NULL)
			{
				perror("Erreur d'ouverture du fichier ");
				resultatTmp.erreur=5;
			}
			//Si le fichier est ouvert sans erreur
			else
			{
				//initialisation de cellule courante
				celluleCourante=calloc(MAXBLOC,sizeof(char));
				celluleCourante->bloc=calloc(MAXBLOC,sizeof(char));
				celluleCourante->suivant=NULL;

				//initialisation de la cellule suivante
				celluleSuivante=celluleCourante;

				//initialisation de resultatTmp
				resultatTmp.read_res_u.fichier=celluleCourante;

				while (fgets(buffer, MAXBLOC, file)!=NULL)
				{
					celluleCourante->suivant=celluleSuivante;

					celluleCourante=celluleSuivante;
					celluleCourante->bloc= calloc(MAXBLOC,sizeof(char));
					celluleCourante->suivant=NULL;

					strcpy(celluleCourante->bloc, buffer);

					celluleSuivante=calloc(MAXBLOC,sizeof(char));
				}
				fclose(file);	
				resultatTmp.erreur=0;
			}
		}
	}
	result = resultatTmp;
	printf("\n\nErreur retournée (0 = OK) : %d\n", result.erreur);
	printf("\n-----------------Fin du programme read-----------------\n\n");
	return &result;
}

int *
write_1_svc(write_parm *argp, struct svc_req *rqstp)
{
	printf("----------------Debut du programme write---------------\n\n");
	static int  result;
	int erreur=1;

	if(erreur<=2)
	{

		erreur=connexionValide(rqstp);

		FILE *file;
		char* fichier=argp->nom;
		cell_bloc *celluleCourante = argp->donnees;

		if(argp->ecraser)
		{
			file = fopen(fichier,"w");
		}
		else
		{
			file = fopen(fichier,"a");
		}
		
		if(file==NULL)
		{
			perror("Erreur d'ouverture du fichier ");
			printf("Fichier : \"%s\" \n",fichier);
			erreur=4;
		}
		else
		{
			while(celluleCourante!=NULL)
			{	
				fputs(celluleCourante->bloc,file);
				celluleCourante=celluleCourante->suivant;
			}
			erreur=0;
			fclose(file);
		}
	}
	result=erreur;
	printf("\n\nErreur retournée (0 = OK) : %d\n", result);
	printf("\n----------------Fin du programme write-----------------\n\n");
	return &result;
}

