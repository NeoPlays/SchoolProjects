//Speichern von Elementen auf eine Datei, Martin Platajs, 3BHEL
#include "header.h"


 void speichern(Kaufvertrag *start,Kaufvertrag *ende)   //UP zum Speichern von Elementen
    {
        FILE *datei;
        char fname[16];
        Kaufvertrag *h;

        printf("Geben Sie einen Filenamen ein!\n");
        scanf("%s",fname);

        datei=fopen(fname,"w");                         //Öffnen der Datei
        if(datei==NULL)
        {
            printf("Error!\n");
        }
        else
        {
            h = start;
            while(h!=NULL)
            {
                fprintf(datei,"%s\n" "%s\n" "%s\n" "%f\n" "%s\n" "%d\n\n",
                        h->nname,h->marke,h->type,h->verbrauch,
                        h->reifendimensionen,h->eigengewicht);  //Einlesen der Daten
                h=h->next;
            }
            printf("Daten wurden gespeichert!");
            fclose(datei);
        }

    }
