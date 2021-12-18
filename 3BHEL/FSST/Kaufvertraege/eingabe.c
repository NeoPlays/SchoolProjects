//Eingabe von Daten eines Elements, Martin Platajs, 3BHEL
#include "header.h"

//UP für das Eingaben der Daten eines Elements
Kaufvertrag eingabe(Kaufvertrag *p)
{
    printf("Geben Sie einen Nachnamen ein!\n");         //Name des Besitzers
    scanf("%s",p->nname);
    printf("Geben Sie eine Marke an!\n");               //Marke des Autos
    scanf("%s",p->marke);
    printf("Geben Sie den Type an!\n");                 //Type des Autos
    scanf("%s",p->type);
    printf("Geben Sie den Verbrauch an!\n");            //Verbrauch des Autos
    scanf("%f",&p->verbrauch);
    printf("Geben Sie die Reifendimensionen an!\n");    //Reifendimension des Autos
    scanf("%s",p->reifendimensionen);
    printf("Geben Sie das Eigengewicht an!\n");         //Eigengewicht des Autos
    scanf("%d",&p->eigengewicht);
    return *p;
}
