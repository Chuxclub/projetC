/********************************************************************
 * Module de Collection : corps
 * Auteurs : Amandine Fradet, Florian Legendre
 ********************************************************************/


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
}

void col_addVoitureAvecTri(Collection self, const_Voiture voiture)
{
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
