//Sortieren von Elementen nach dem Nachnamen, Martin Platajs, 3BHEL
#include "header.h"
#include <string.h>

void sortieren(Kaufvertrag **start,Kaufvertrag **ende,Kaufvertrag *p)       //UP für das Sortieren von Elementen nach dem Nachnamen
{
    Kaufvertrag* h;

    if((*start)==NULL)                              //Wenn in der Liste noch kein Element existiert,
    {                                               //muss sowohl Start- als auch End-Pointer gesetzt werden.
        (*start) = p;
        (*ende) = p;
        p->next = NULL;
        p->prev = NULL;
    }
    else                                            //Alle anderen Fälle werden nur abgeprüft wenn Elemente bereits vorhanden sind
    {
        if(strcmp((*start)->nname,p->nname) >= 0)   //Wenn das hinzugefuegte Element ganz vorne einsortiert gehoert.
        {                                           //Bei Namensgleichheit des ersten Elements wird eine Ausnahme benötigt,
            p->next = (*start);                     //da der Start-Pointer veraendert werden muss, deshalb ein ">=".
            p->prev = NULL;
            p->next->prev = p;
            (*start) = p;
        }
        else if(strcmp((*ende)->nname,p->nname) < 0)  //Wenn das hinzugefuegte Element am Ende einsortiert gehoert
        {
            p->next = NULL;
            p->prev = (*ende);
            p->prev->next = p;
            (*ende) = p;
        }
        else                                          //Wenn keine Ausnahme zutrifft wird das Element normal einsortiert.
        {                                             //Bei Namensgleichheit wird normal einfach davor einsortiert,
                                                      //ausser es betrifft das erste Element.
            h = (*start);
            while(strcmp(h->nname,p->nname) < 0)      //Schleife zum Eruieren der Position
            {
                h=h->next;
            }

            h->prev->next = p;
            p->prev = h->prev;
            h->prev = p;
            p->next = h;

        }

    }
}
