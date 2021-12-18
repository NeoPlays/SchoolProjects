//Ausgabe der Daten eines Elements, Martin Platajs, 3BHEL
#include "header.h"

void ausgabe(Kaufvertrag *p)        //UP für die Ausgabe der Daten eines Elements
{
    printf("\n%s\n",p->nname);                  //Ausgabe des Nachnamens
    printf("%s\n",p->marke);                    //Ausgabe der Marke
    printf("%s\n",p->type);                     //Ausgabe des Typs
    printf("%f Liter/100km\n",p->verbrauch);    //Ausgabe des Verbrauchs in Liter/100km
    printf("%s\n",p->reifendimensionen);        //Ausgabe der Reifendimensionen
    printf("%d kg\n",p->eigengewicht);          //Ausgabe des Eigengewichts
}

