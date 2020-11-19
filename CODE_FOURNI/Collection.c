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

bool hasPrevious(VoitureCell cell)
{
	return cell->previousCell != NULL;
}

bool hasNext(VoitureCell cell)
{
	return cell->nextCell != NULL;
}

VoitureCell next(VoitureCell cell)
{
	return cell->nextCell;
}

VoitureCell previous(VoitureCell cell)
{
	return cell->previousCell;
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
	assert(source->len > 0);

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
	assert(pos >= 0 && pos < self->len);

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
	assert(self->isSorted);
	int n = self->len;
	assert(n >= 0);

	//Création de la nouvelle cellule:
	VoitureCell newCell = createVoitureCell(voiture);

	if(n == 0)
	{
		//Branchements des champs première/dernière cellule
		//de la structure CollectionP pointée par Collection:
		self->firstCell = newCell;
		self->lastCell = newCell;
	}

	else if(n == 1)
	{
		int year_v0 = voi_getAnnee(self->firstCell->v);
		int year_v1 = voi_getAnnee(voiture);

		if(year_v1 <= year_v0)
		{
			//Branchements des cellules entre elle:
			newCell->nextCell = self->firstCell;
			self->firstCell->previousCell = newCell;

			//Branchements des champs:
			self->firstCell = newCell;
		}

		else
		{
			//Branchements des cellules entre elle:
			newCell->previousCell = self->firstCell;
			self->firstCell->nextCell = newCell;

			//Branchements des champs:
			self->lastCell = newCell;
		}
	}

	else
	{
		VoitureCell currentCell = self->firstCell;
		int year_current_v = voi_getAnnee(currentCell->v);
		int year_v = voi_getAnnee(voiture);
		
		while(year_v >= year_current_v && hasNext(currentCell))
		{
			currentCell = next(currentCell);
			year_current_v = voi_getAnnee(currentCell->v);
		}

		if(currentCell == self->firstCell)
		{
			newCell->nextCell = self->firstCell;
			self->firstCell->previousCell = newCell;
			self->firstCell = newCell;
		}

		else if(currentCell == self->lastCell)
		{
			newCell->previousCell = self->lastCell;
			self->lastCell->nextCell = newCell;
			self->lastCell = newCell;
		}

		else
		{
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
	//Vérification de la position et de la présence de quelque chose à supprimer:
	myassert((pos >= 0 && pos < self->len), "La position entrée n'est pas correcte");
	myassert((self->len > 0), "Il n'y a rien à supprimer");

	//Première et unique cellule sélectionnée:
	if(self->len == 1)
	{
		col_vider(self);
	}

	else
	{
		VoitureCell currentCell = self->firstCell;
		int i = 1;

		//Déplacement dans la collection:
		while(i != pos)
		{
			currentCell = next(currentCell);
			i++;
		}
		
		//Première cellule:
		if(currentCell == self->firstCell)
		{
			VoitureCell nextCell = currentCell->nextCell;

			nextCell->previousCell = NULL;
			self->firstCell = nextCell;

			freeVoitureCell(currentCell);
		}

		//Dernière cellule:
		else if(currentCell == self->lastCell)
		{
			VoitureCell previousCell=currentCell->previousCell;

			previousCell->nextCell = NULL;
			self->lastCell = previousCell;

			freeVoitureCell(currentCell);
		}

		//N'importe quelle autre cellule:
		else
		{
			VoitureCell previousCell=currentCell->previousCell;
			VoitureCell nextCell = currentCell->nextCell;

			//Relie la cellule précédente et suivante:
			previousCell->nextCell = nextCell;
			nextCell->previousCell = previousCell;
			freeVoitureCell(currentCell);
		}
	}

	self->len--;
	   
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void col_supprVoitureAvecTri(Collection self, int pos)
{
	assert(pos >= 0 && pos < self->len);
	assert(self->len > 0);

	if(self->len == 1)
	{
		freeVoitureCell(self->firstCell);
		self->firstCell = NULL;
		self->lastCell = NULL;
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
	assert(self->len > 0);

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

	if (pvoit == NULL) 
		printf("Il n'y a aucune voiture à afficher car la collection est vide\n");
	else
	{
		printf("Collection de %d voiture(s)\n", self->len);
		//tant que pvoit n'est pas null
   		while(pvoit)
   		{
   			voi_afficher(pvoit->v);
   			pvoit = pvoit->nextCell;
   		}
	}

	if (self->isSorted) 
		printf("isSorted : True\n");
    
	else 
		printf("isSorted : False\n");
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
	int n = 0;
	fread(&(self->isSorted), sizeof(bool), 1, fd);
	fread(&n, sizeof(int), 1, fd);

	//On récupère les voitures du fichier selon ces deux
	//paramètres évoqués précédemment. self->len est 
	//incrémenté dans col_addVoiture avec ou sans tri:
	

	Voiture current_v = voi_creerFromFichier(fd);
	
	//On doit créer notre première cellule en-dehors de
	//col_addVoiture si on veut éviter de fausser les statistiques
	//par une copie excédante:
	VoitureCell newCell = (VoitureCell) malloc(sizeof(struct VoitureCellP));
	newCell->previousCell = NULL;
	newCell->v = current_v;
	newCell->nextCell = NULL;

	self->firstCell = newCell;
	self->lastCell = newCell;
	self->len++;
	
	//On peut maintenant copier avec col_addVoiture sans problème:
	if(self->isSorted)
	{
		for(int i = 1; i < n; i++)
		{
			voi_lireFichier(current_v, fd);
			col_addVoitureAvecTri(self, current_v);
		}
	}

	else
	{
		for(int i = 1; i < n; i++)
		{
			voi_lireFichier(current_v, fd);
			col_addVoitureSansTri(self, current_v);
		}
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/* ===================================================================== */
/* ===================================================================== */
/* ===================================================================== */
