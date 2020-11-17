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
		self->len++;
		
		//Création de la nouvelle cellule:
		VoitureCell newCell = (VoitureCell) malloc(sizeof(*VoitureCell));
		newCell->previousCell = NULL;
		newCell->v = voi_creerCopie(voiture);
		newCell->nextCell = NULL;
	}

	else if(n == 1)
	{
	}

	else
	{
	}
}

void col_supprVoitureSansTri(Collection self, int pos)
{
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
