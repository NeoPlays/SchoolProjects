//Suchen nach Elementen nach dem jeweiligen Parameter, Martin Platajs, 3BHEL
#include "header.h"
#include <string.h>
#include <conio.h>

void suchen(Kaufvertrag **start,Kaufvertrag **ende,int parameter)
{                                           //UP zum Suchen von Elementen
    char such[16];
    float verbrauch;
    int counter=0,select=0;
    Kaufvertrag* h;
    h = (*start);

    switch(parameter)           //Auswahl zwischen den uebergebenen Parameter
    {
        case '6':                                       //Suchen nach Typ
            printf("Nach welchem Type wollen Sie suchen?\n");
            scanf("%s",such);
            printf("Alle Elemente mit dem Type %s:\n",such);

            while(h!=NULL)
            {
                if(strcmp(such,h->type)==0)
                {
                    ausgabe(h);                         //Aufruf des UPs ausgabe
                    counter ++;
                }
                h=h->next;
            }
            printf("\n Es wurden %d Elemente mit dem Type %s gefunden!\n",
                   counter,such);

            break;

        case '7':                                      //Suchen nach Verbrauch
            printf("Nach welchem Verbrauch wollen Sie suchen?\n");
            scanf("%f",&verbrauch);
            printf("Alle Elemente mit dem Verbrauch %f:\n",verbrauch);

            while(h!=NULL)
            {
                if(verbrauch==h->verbrauch)
                {
                    ausgabe(h);                         //Aufruf des UPs ausgabe
                    counter ++;
                }
                h=h->next;
            }
            printf("\n Es wurden %d Elemente mit dem Verbrauch %f gefunden!\n",
                   counter,verbrauch);

            break;

        case '8':                                       //Suchen nach Nachname
            printf("Nach welchem Nachnamen wollen sie loeschen?\n");
            scanf("%s",such);
            while(h!=NULL)
            {
                 if(strcmp(such,h->nname)==0)
                 {
                    ausgabe(h);                         //Aufruf des UPs ausgabe
                    printf("Wollen Sie dieses Element loeschen? 1/0");
                    scanf("%d",&select);

                        if(select==1)
                        {
                            loeschen(start,ende,h);     //Aufruf des UPs loeschen
                            counter++;
                        }
                 }
                 h=h->next;
            }
            printf("\n Es wurden %d Elemente mit dem Nachnamen %s geloescht!\n",
                   counter,such);
            break;

        case '9':                                       //Suchen nach Reifendimensionen
            printf("Nach welchen Reifendimensionen wollen sie loeschen?\n");
            scanf("%s",such);

            while(h!=NULL)
            {
                 if(strcmp(such,h->reifendimensionen)==0)
                 {
                    ausgabe(h);                         //Aufruf des UPs ausgabe
                    printf("Wollen Sie dieses Element loeschen? 1/0");
                    scanf("%d",&select);

                        if(select==1)
                        {
                            loeschen(start,ende,h);     //Aufruf des UPs loeschen
                            counter++;
                        }
                 }
                 h=h->next;
            }
            printf("\n Es wurden %d Elemente mit den Reifendimensionen %s geloescht!\n",
                   counter,such);
            break;



    }

}
