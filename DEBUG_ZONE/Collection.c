/********************************************************************
 * Module de Collection : corps
 * Auteurs : Amandine Fradet, Florian Legendre
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <assert.h>
#include "myassert.h"

#include "Collection.h"

struct VoitureCellP
{
	struct VoitureCellP* previousCell;
	Voiture v;
	struct VoitureCellP* nextCell;
};

typedef struct VoitureCellP* VoitureCell;

struct CollectionP
{
	VoitureCell firstCell;
	int len;
	int isSorted;
	VoitureCell lastCell;
};


/* ===================================================================== */
/* ========= FONCTIONS SPECIFIQUES AUX CELLULES DE LA LISTE  =========== */
/* ===================================================================== */

VoitureCell createVoitureCell(const_Voiture voiture)
{
	VoitureCell res = (VoitureCell) malloc(sizeof(struct VoitureCellP));
	res->previousCell = NULL;
   	res->v = voi_creerCopie(voiture);
   	res->nextCell = NULL;

	return res;
}

void freeVoitureCell(VoitureCell cell)
{
	voi_detruire(&(cell->v));
	free(cell);
}

VoitureCell next(VoitureCell cell)
{
	return cell->nextCell;
}


/* ===================================================================== */
/* ===================================================================== */
/* ===================================================================== */







/* ===================================================================== */
/* ================== FONCTIONS SPECIFIQUES A LA LISTE ================= */
/* ===================================================================== */




		/* ------------------------------
 		 * Initialisation de la structure
		 * ------------------------------ */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Collection col_creer()
{
	Collection res = (Collection) malloc(sizeof(struct CollectionP));
	res->firstCell = NULL;
	res->len = 0;
	res->isSorted = true;
	res->lastCell = NULL;

	return res;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Collection col_creerCopie(const_Collection source)
{
	//On vérifie qu'il y a quelque-chose à copier:
	myassert(source->len > 0, "La source ne dispose d'aucune collection à copier");

	//On crée la collection qui dans laquelle on 
	//va copier la collection source:
	Collection res = col_creer();

	//On a besoin de naviguer dans la collection source: 
	VoitureCell current_srcCell = source->firstCell;

	//On ne fait pas confiance à la fonction d'ajout sans
	//tri pour copier notre colonne en gardant le tri:
	if(source->isSorted)
	{
		do
		{
			col_addVoitureAvecTri(res, current_srcCell->v);
			current_srcCell = next(current_srcCell);

		} while(current_srcCell != NULL);
	}

	else
	{
		do
		{
			col_addVoitureSansTri(res, current_srcCell->v);
			current_srcCell = next(current_srcCell);

		} while(current_srcCell != NULL);

	}
	
	//Finalement, notre collection copiée sera triée ou non
	//et on peut retourner la copie ainsi faite:
	res->isSorted = source->isSorted;
	return res;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void col_detruire(Collection *pself)
{
	VoitureCell tmp;
	VoitureCell pvoit = (*pself)->firstCell;

	while(pvoit)
	{
		//sauvegarde de la cellule actuelle
		tmp = pvoit;
		//sauvegarde de la prochaine cellule
		pvoit = pvoit->nextCell;
		//libère la mémoire de tmp
		freeVoitureCell(tmp);
	}

	free(*pself);
	*pself = NULL;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void col_vider(Collection self)
{
	self->len = 0;
	self->isSorted = true;

	VoitureCell tmp;
	VoitureCell pvoit = self->firstCell;

	while(pvoit)
	{
		//sauvegarde de la cellule actuelle
		tmp = pvoit;
		//sauvegarde de la prochaine cellule
		pvoit = pvoit->nextCell;
		//libère la mémoire de tmp
		freeVoitureCell(tmp);
	}
	
	self->firstCell = NULL;
	self->lastCell = NULL;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





				/*-----------*
 				 * accesseurs
				 * ----------*/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int col_getNbVoitures(const_Collection self)
{
	return self->len;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Voiture col_getVoiture(const_Collection self, int pos)
{
	myassert(pos >= 0 && pos < self->len, "La position n'est pas correcte");

	VoitureCell current_cell = self->firstCell;

	for(int i = 0; i < pos; i++)
		current_cell = next(current_cell);

	Voiture res = voi_creerCopie(current_cell->v);

	return res;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void col_addVoitureSansTri(Collection self, const_Voiture voiture)
{
	//On pourra toujours ajouter la voiture...
	//Donc pas d'assert! 

	//Création de la nouvelle cellule:
	VoitureCell newCell = createVoitureCell(voiture);

	if(self->len == 0)
	{
		//Branchement des champs:
		self->firstCell = newCell;
		self->lastCell = newCell;
	}

	else
	{
		//Branchement des cellules entre elles (ajout en queue):
		self->lastCell->nextCell = newCell;
		newCell->previousCell = self->lastCell;

		//Branchement du champ (ajout en queue):
		self->lastCell = newCell;
		
		//La collection devient non triée contrairement au cas
		//où il n'y avait initialement pas d'élément:
		self->isSorted = false;
	}

	self->len++;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void col_addVoitureAvecTri(Collection self, const_Voiture voiture)
{
	//On n'ajoute pas avec une fonction d'ajout avec tri de complexité
	//linéaire dans une collection non triée (la fonction d'ajout sans
	//tri a une complexité constante:
	myassert(self->isSorted, "La collection n'est pas triée");

	//Par conception la longueur devrait toujours être >= 0 ...
	//Si cet assert est invalidé il y a un sérieux problème:
	int n = self->len;
	myassert(n >= 0, "La taille de la collection est inférieure à 0");

	//Création de la nouvelle cellule:
	VoitureCell newCell = createVoitureCell(voiture);
	
	//Si la collection ne contient initialement aucune voiture on a
	//des branchements particuliers à faire:
	if(n == 0)
	{
		//Branchements des champs première/dernière cellule
		//de la structure CollectionP pointée par Collection:
		self->firstCell = newCell;
		self->lastCell = newCell;
	}

	//On pose v' la voiture qu'on souhaite ajouter et vk une
	//voiture de la collection à la position k:
	else
	{
		VoitureCell currentCell = self->firstCell;
		int year_current_v = voi_getAnnee(currentCell->v);
		int year_v = voi_getAnnee(voiture);

		//Cas où v' < v1:
		if(year_v < voi_getAnnee(self->firstCell->v))
		{
			newCell->nextCell = self->firstCell;
			self->firstCell->previousCell = newCell;
			self->firstCell = newCell;
		}

		//Cas où v' >= vn:
		else if(year_v >= voi_getAnnee(self->lastCell->v))
		{
			newCell->previousCell = self->lastCell;
			self->lastCell->nextCell = newCell;
			self->lastCell = newCell;

		}

		//Cas où v1 <= v' < vn:
		else
		{
			while(year_v >= year_current_v)
			{
				currentCell = next(currentCell);
				year_current_v = voi_getAnnee(currentCell->v);
			}

			newCell->nextCell = currentCell;
			newCell->previousCell = currentCell->previousCell;

			currentCell->previousCell->nextCell = newCell;
			currentCell->previousCell = newCell;
		}
	}

	//Incrémentation de la taille de la collection:
	self->len++;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void col_supprVoitureSansTri(Collection self, int pos)
{
	col_supprVoitureAvecTri(self, pos);

	if(self->len == 1)
		self->isSorted = true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void col_supprVoitureAvecTri(Collection self, int pos)
{
	//Vérification de la position et de la présence 
	//de quelque chose à supprimer:
	myassert((pos >= 0 && pos < self->len), "Position incorrecte");
	myassert((self->len > 0), "Il n'y a rien à supprimer");
	

	if(self->len == 1)
	{
		col_vider(self);
	}

	else
	{
		VoitureCell currentCell = self->firstCell;

		for(int i = 0; i < pos; i++)
			currentCell = next(currentCell);
		
		if(currentCell == self->firstCell)
		{
			VoitureCell nextCell = currentCell->nextCell;

			nextCell->previousCell = NULL;
			self->firstCell = nextCell;

			freeVoitureCell(currentCell);
		}

		else if(currentCell == self->lastCell)
		{
			VoitureCell previousCell=currentCell->previousCell;

			previousCell->nextCell = NULL;
			self->lastCell = previousCell;

			freeVoitureCell(currentCell);
		}

		else
		{
			VoitureCell previousCell=currentCell->previousCell;
			VoitureCell nextCell = currentCell->nextCell;

			previousCell->nextCell = nextCell;
			nextCell->previousCell = previousCell;
			freeVoitureCell(currentCell);
		}
	}

	self->len--;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void col_trier(Collection self)
{
	//On vérifie qu'il y a au moins 
	//un élément à trier:
	myassert(self->len > 0, "Il n'y a aucun élément à trier");

	//On fait un tri à bulle:
	int list_end = self->len;

	VoitureCell current_cell = NULL;
	VoitureCell next_cell = NULL;
	int year_currentV = 0;
	int year_nextV = 0;

	for(int i = 0; i < list_end; i++)
	{
		current_cell = self->firstCell;
		next_cell = self->firstCell->nextCell;

		for(int j = 0; j < list_end - 1 - i; j++)
		{
			year_currentV = voi_getAnnee(current_cell->v);
			year_nextV = voi_getAnnee(next_cell->v);

			if(year_currentV > year_nextV)
			{
				voi_swap(current_cell->v, next_cell->v);
			}

			current_cell = next(current_cell);
			next_cell = next(next_cell);
		}
	}

	//La collection est alors triée:
	self->isSorted = true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




			/*-------------------------------*
 			 * méthode secondaire d'affichage
 			 * ------------------------------*/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void col_afficher(const_Collection self)
{
	VoitureCell pvoit = self->firstCell;

	printf("Collection: \n");
	printf("Il y a %d voiture(s)\n", self->len);
	
	if (self->isSorted) 
		printf("La collection est triée\n");
    
	else 
		printf("La collection n'est pas considérée comme triée\n");


	//tant que pvoit n'est pas null
   	while(pvoit)
   	{
   		voi_afficher(pvoit->v);
   		pvoit = pvoit->nextCell;
   	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





		/*-----------------------------------------------*
 		 * entrées-sorties fichiers
 		 * note : le paramètre est un fichier déjà ouvert
 		 * ----------------------------------------------*/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void col_ecrireFichier(const_Collection self, FILE* fd)
{
	fwrite(&(self->isSorted), sizeof(bool), 1, fd);
	fwrite(&(self->len), sizeof(int), 1, fd);

	VoitureCell currentCell = self->firstCell;

	for(int i = 0; i < self->len; i++)
	{
		voi_ecrireFichier(currentCell->v, fd);
		currentCell = next(currentCell);
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void col_lireFichier(Collection self, FILE* fd)
{
	//On vide la collection pour ne laisser
	//que ce qu'on veut copier:
	col_vider(self);

	//On récupère deux informations essentielles:
	//Le nombre 'n' de voitures dans le fichier
	//et si la collection est supposée être triée!
	fread(&(self->isSorted), sizeof(bool), 1, fd);

	int n = 0;
	fread(&n, sizeof(int), 1, fd);

	//On récupère les voitures du fichier selon ces deux
	//paramètres évoqués précédemment. self->len est 
	//incrémenté dans col_addVoiture avec ou sans tri:

	Voiture current_v = voi_creerFromFichier(fd);

	//On peut maintenant copier avec col_addVoiture sans problème:
	if(self->isSorted)
	{
		col_addVoitureAvecTri(self, current_v);

		for(int i = 1; i < n; i++)
		{
			voi_lireFichier(current_v, fd);
			col_addVoitureAvecTri(self, current_v);
		}
	}

	
	else
	{
		col_addVoitureSansTri(self, current_v);

		for(int i = 1; i < n; i++)
		{
			voi_lireFichier(current_v, fd);
			col_addVoitureSansTri(self, current_v);
		}
	}

	//On libère la zone mémoire de notre voiture 
	//current_v qu'on a allouée avec voi_creerFromFichier():
	voi_detruire(&current_v);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/* ===================================================================== */
/* ===================================================================== */
/* ===================================================================== */
