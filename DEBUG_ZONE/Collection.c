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
/* ======== FONCTIONS SPECIFIQUES A LA LISTE DOUBLEMENT CHAINEE ======== */
/* ===================================================================== */

VoitureCell createVoitureCell(const_Voiture voiture)
{
	VoitureCell res = malloc(sizeof(struct VoitureCellP));
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

Collection col_creer()
{
	Collection res = (Collection) malloc(sizeof(struct CollectionP));
	res->firstCell = NULL;
	res->len = 0;
	res->isSorted = true;
	res->lastCell = NULL;

	return res;
}

/*
Collection col_creerCopie(const_Collection source)
{
	Collection res = col_creer();
	
	res->len = source->len;
	res->isSorted = source->isSorted;

	return res;
}
*/

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



/*-----------*
 * accesseurs
 * ----------*/

int col_getNbVoitures(const_Collection self)
{
	return self->len;
}


Voiture col_getVoiture(const_Collection self, int pos)
{
	assert(pos >= 0 && pos < self->len);

	VoitureCell current_cell = self->firstCell;

	for(int i = 0; i < pos; i++)
		current_cell = next(current_cell);

	Voiture res = voi_creerCopie(current_cell->v);

	return res;
}


void col_addVoitureSansTri(Collection self, const_Voiture voiture)
{
	//On pourra toujours ajouter la voiture... 
	//Et la collection deviendra non triée:
	self->isSorted = false;

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
	}

	self->len++;
}


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

void col_supprVoitureSansTri(Collection self, int pos)
{
	//on part du principe que la position est toujours strictement positive
	if ((pos > self->len) || (pos != 0)) 
		printf("Vous ne pouvez rien supprimer");

   	else
	   {
		   VoitureCell tmp = self->firstCell;
		   int i = 1;
		  
		   while(i != pos)
		   {
			   tmp = tmp->nextCell;
			   i++;
		   }

			//relie la cellule précédente et la suivante
			previous(tmp)->nextCell = next(tmp);
			next(tmp)->previousCell = previous(tmp);
	
   			free(tmp);
	   }
}


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


/*--------------*
 * méthode secondaire d'affichage
 * -------------*/
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


/*-------------*
 * entrées-sorties fichiers
 * note : le paramètre est un fichier déjà ouvert
 * ------------*/
/*
void col_ecrireFichier(const_Collection self, FILE* fd)
{
}

void col_lireFichier(Collection self, FILE* fd)
{
}
*/
