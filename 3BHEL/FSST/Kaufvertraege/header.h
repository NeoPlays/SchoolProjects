//Das Headerfile des ganzen Projekts, Martin Platajs, 3BHEL
#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

typedef struct Kaufvertrag  //Struktur
{
    char nname[16];
    char marke[16];
    char type[16];
    float verbrauch;
    char reifendimensionen[20];
    int eigengewicht;
    struct Kaufvertrag* next;
    struct Kaufvertrag* prev;
}Kaufvertrag;

//Prototypen
Kaufvertrag eingabe(Kaufvertrag *p);
void sortieren(Kaufvertrag **start,Kaufvertrag **ende,Kaufvertrag *p);
void erstellen(Kaufvertrag **start,Kaufvertrag **ende);
void ausgabe(Kaufvertrag *h);
void ausgabeaufsteig(Kaufvertrag *start);
void ausgabeabsteig(Kaufvertrag *ende);
void einlesen(Kaufvertrag **start,Kaufvertrag **ende);
void speichern(Kaufvertrag *start,Kaufvertrag *ende);
void suchen(Kaufvertrag **start,Kaufvertrag **ende,int parameter);
void loeschen(Kaufvertrag **start,Kaufvertrag **ende,Kaufvertrag *h);
void menue();
void auswahl();




#endif // HEADER_H_INCLUDED

