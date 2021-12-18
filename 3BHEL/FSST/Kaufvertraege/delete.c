//Löschen von Elementen, Martin Platajs, 3BHEL
#include "header.h"

void loeschen(Kaufvertrag **start,Kaufvertrag **ende,Kaufvertrag *h)
{
    if(h==(*start) && h==(*ende))   //Wenn nur 1 Element existiert
    {
        (*ende)=NULL;
        (*start)=NULL;
        free(h);
        h=NULL;
    }
    else if(h==(*start))            //Wenn das Element das 1. ist
    {
        (*start)->next->prev=NULL;
        (*start)=h->next;
        free(h);
        h=NULL;
    }
    else if(h==(*ende))             //Wenn das Element das letzte ist
    {
        (*ende)->prev->next=NULL;
        (*ende)=h->prev;
        free(h);
        h=NULL;

    }
    else                            //In allen anderen Fällen
    {
        h->prev->next=h->next;
        h->next->prev=h->prev;
        free(h);
        h=NULL;
    }
}
