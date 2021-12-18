//Einlesen von Elementen von einer Datei, Martin Platajs, 3BHEL
#include "header.h"

//UP zum Einlesen von Elementen von einer Datei
void einlesen(Kaufvertrag **start,Kaufvertrag **ende)
{
    FILE *datei;
    Kaufvertrag *p = NULL;
    char fname[16];

    printf("Geben sie einen Filenamen ein!\n");
    scanf("%s",fname);

    datei=fopen(fname,"r");                                 //Öffnen einer Datei
    if(datei==NULL)
    {
            printf("ERROR!\n");
    }
    else
    {
        p=(Kaufvertrag*)malloc(sizeof(Kaufvertrag));
        if(p==NULL)
        {
            printf("Error!\n");
        }
        else
        {
            while((fscanf(datei,"%s" "%s" "%s" "%f" "%s" "%d",p->nname,p->marke,p->type,
                    &p->verbrauch,p->reifendimensionen,&p->eigengewicht)>0)&&(p!=NULL))
                                                            //Einlesen von Datei
                {
                    sortieren(start,ende,p);                //Aufruf des UPs sortieren
                    p=(Kaufvertrag*)malloc(sizeof(Kaufvertrag));
                }
                if(p==NULL)
                {
                    printf("Error!\n");
                }
                else
                {
                    printf("Daten wurden eingelesen!");
                }
            fclose(datei);
        }


    }

}
