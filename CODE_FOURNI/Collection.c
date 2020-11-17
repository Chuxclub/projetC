/********************************************************************
 * Module de Collection : corps
 * Auteurs : Amandine Fradet, Florian Legendre
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
	VoitureCell res = malloc(sizeof(*VoitureCell));
	res->previousCell = NULL;
   	res->v = voi_creerCopie(voiture);
   	res->nextCell = NULL;

	return res;
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
	Collection res = (Collection) malloc(sizeof(*Collection));
	res->firstCell = NULL;
	res->len = 0;
	res->isSorted = true;
	res->lastCell = NULL;
}

Collection col_creerCopie(const_Collection source)
{
	Collection res = col_creer();
	
	res->len = source->len;
	res->isSorted = source->isSorted;
}

void col_detruire(Collection *pself)
{
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
		free(tmp);
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
}

void col_addVoitureSansTri(Collection self, const_Voiture voiture)
{
	self->len ++;
	
	VoitureCell new = malloc(sizeof(VoitureCell));
   	new->v = voi_creerCopie(voiture);
   	new->previousCell = self->lastCell;
   	new->nextCell = NULL;
	
	//cas où la collection est vide
	if (col_getNbVoitures(self) != 0) self->lastCell->nextCell = new;
   	else 
	   {
			self->firstCell = new;
   			self->lastCell = new;
	   }  
   
	self->isSorted = false;
}

void col_addVoitureAvecTri(Collection self, const_Voiture voiture)
{
	assert(self->isSorted);
	int n = self->len;
	assert(n > 0);

	if(n == 0)
	{
		//Création de la nouvelle cellule:
		VoitureCell newCell = createVoitureCell(voiture);

		//Branchements des champs première/dernière cellule
		//de la structure CollectionP pointée par Collection:
		self->firstCell = newCell;
		self->lastCell = newCell;

		//Incrémentation de la taille de la collection:
		self->len++;
	}

	else if(n == 1)
	{
		//Création de la nouvelle cellule:
		VoitureCell newCell = createVoitureCell(voiture);

		int year_v0 = voi_getAnnee(self->firstCell->v);
		int year_v1 = voi_getAnnee(voiture);

		if(year_v0 <= year_v1)
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


		//Incrémentation de la taille de la collection:
		self->len++;

	}

	else
	{
		//Création de la nouvelle cellule:
		VoitureCell newCell = createVoitureCell(voiture);

		VoitureCell currentCell = self->firstCell;
		int year_current_v = voi_getAnnee(currentCell->v);
		int year_v = voi_getAnnee(voiture);
		
		while(year_current_v >= year_v && hasNext(currentCell))
		{
			currentCell = next(currentCell);
			year_current_v = voi_getAnnee(currentCell->v);
		}

		//Branchement des cellules entre elles
		if(currentCell == self->firstCell)
		{
			newCell->nextCell = self->firstCell;
			self->firstCell->previousCell = newCell;
			self->firstCell = newCell;
		}

		else if(currentCell == self->lastCell)
		{
			
		}

		else
		{
			newCell->nextCell
		}

		//Incrémentation de la taille de la collection:
		self->len++;
	}
}

void col_supprVoitureSansTri(Collection self, int pos)
{
	//on part du principe que la position est toujours strictement positive
	if ((pos > self->len) || (pos != 0)) printf("Vous ne pouvez rien supprimer");
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
}

void col_trier(Collection self)
{
}


/*--------------*
 * méthode secondaire d'affichage
 * -------------*/
void col_afficher(const_Collection self)
{
	VoitureCell pvoit = self->firstCell;

	if (pvoit == NULL) printf("Il n'y a aucune voiture à afficher car la collection est vide\n");
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

	if (self->isSorted) printf("isSorted : True\n");
    else printf("isSorted : False\n");
}


/*-------------*
 * entrées-sorties fichiers
 * note : le paramètre est un fichier déjà ouvert
 * ------------*/

void col_ecrireFichier(const_Collection self, FILE* fd)
{
}

void col_lireFichier(Collection self, FILE* fd)
{
}
