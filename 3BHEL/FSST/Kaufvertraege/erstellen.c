//Erstellen von Elementen, Martin Platajs, 3BHEL
#include "header.h"
#include "malloc.h"

void erstellen(Kaufvertrag **start,Kaufvertrag **ende)      //UP zum Erstellen von Elementen
{
    int wh=1;
    Kaufvertrag* p;
    do
    {
        p=(Kaufvertrag*)malloc(sizeof(Kaufvertrag));        //Speicher reservieren
        if(p == NULL)                                       //Überprüfung der Reservierung
        {
            printf("Kein Speicher!");
        }
        else
        {
            *p=eingabe(p);                                  //Aufruf des "Eingabe" UPs
            sortieren(start,ende,p);                        //Aufruf des "Ausgabe" UPs
        }

        printf("Wiederholen? 1/0");
        scanf("%d",&wh);
    }
    while(wh != 0);                         //Schleife zum Wiederholen dieses Vorgangs
}
