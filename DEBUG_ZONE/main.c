#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "myassert.h"

#include "Voiture.h"
#include "Collection.h"


/*=================================================================*
 * Programme principal
 *=================================================================*/
int main()
{
    Collection c1 = col_creer();
    Voiture v;
    const char * plaques[] = {
            "ZA 123 AZ",
            "ZF 874 FZ",
            "ZM 111 MZ",
            "ZC 642 CZ",
            "ZK 914 KZ",
            "ZX 547 XZ",
            "ZH 733 HZ",
            "ZP 283 PZ",
            "ZB 881 BZ",
            "ZL 313 LZ"
        };
    
    // ajout de 3 voitures en respectant le tri
    v = voi_creer("Orchidile", 2011, 8513, 2, plaques+5);
    col_addVoitureAvecTri(c1, v);
    voi_detruire(&v);

    v = voi_creer("Orwelline", 1984, 433813, 1, plaques+7);
    col_addVoitureAvecTri(c1, v);
    voi_detruire(&v);

    v = voi_creer("Colectine", 1902, 37, 1, plaques+8);
    col_addVoitureAvecTri(c1, v);
    voi_detruire(&v);
    
    col_afficher(c1);

    return EXIT_SUCCESS;
}

