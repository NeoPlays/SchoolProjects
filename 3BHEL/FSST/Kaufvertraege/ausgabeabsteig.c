//Ausgabe der Elemente von Ende bis Start in der Konsole, Martin Platajs, 3BHEL
#include "header.h"

//UP zum Ausgaben der Elemente von Ende bis Start
void ausgabeabsteig(Kaufvertrag *ende)
{
    Kaufvertrag *h;
    h = ende;
    while(h != NULL)
    {
        ausgabe(h);
        h=h->prev;
    }
}
