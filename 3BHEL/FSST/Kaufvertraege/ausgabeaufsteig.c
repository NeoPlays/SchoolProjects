//Ausgabe der Elemente von Start bis Ende in der Konsole, Martin Platajs, 3BHEL
#include "header.h"

//UP zum Ausgaben der Elemente von Start bis Ende
void ausgabeaufsteig(Kaufvertrag *start)
{
    Kaufvertrag *h;
    h = start;
    while(h != NULL)
    {
        ausgabe(h);
        h=h->next;
    }
}
