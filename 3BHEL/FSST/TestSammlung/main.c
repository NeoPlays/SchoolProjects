//Martin Platajs 3BHEL 19.9.18
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#inclide <windows.h>


typedef struct Date
{
    int tag;
    int monat;
    int jahr;
}Date;

typedef struct Test
{
    Date datum;
    char gegenstand[20];
    int punkte;
}Test;

void menu();
void eingeben(Test *const arr, int *const anz);
void ausgeben(Test *const arr, int *const anz);
void einlesen(Test *const arr, int *const anz);
void speichern(Test *const arr, int *const anz);
void durchschnitt(Test *const arr, int *const anz);


void menu()
{
    printf("1.Eingeben\n");
    printf("2.Ausgeben\n");
    printf("3.Einlesen\n");
    printf("4.Speichern\n");
    printf("5.Durchschnitt\n");
    printf("0.Ende\n");
}

void eingeben(Test *const arr,int *const anz)
{
    do
        {
            puts("Wieviel Tests wollen sie eingben?(max 20)");
            scanf("%d",anz);
        }while(*anz>20);
    for(int i=0; i<(*anz); i++)
    {
        printf("\n%d.Test:\n",i+1);
        puts("Tag:");
        scanf("%d",&arr[i].datum.tag);

        puts("Monat:");
        scanf("%d",&arr[i].datum.monat);

        puts("Jahr:");
        scanf("%d",&arr[i].datum.jahr);

        puts("Gegenstand:");
        scanf("%s",arr[i].gegenstand);

        puts("Punkte:");
        scanf("%d",&arr[i].punkte);
    }


}
void ausgeben(Test *const arr, int *const anz)
{
    for(int i=0; i<(*anz); i++)
    {
        printf("\n%d.Test:\n",i+1);
        printf("Datum: %d.%d.%d\n",arr[i].datum.tag,arr[i].datum.monat,arr[i].datum.jahr);
        printf("Gegenstand: %s\n",arr[i].gegenstand);
        printf("Punkte: %d\n",arr[i].punkte);
    }
}

void einlesen(Test *const arr, int *const anz)
{
    FILE *datei;
    char file[10];
    scanf("%s",file);
    printf("Geben Sie einen Filenamen ein!\n");
    datei=fopen(file,"r");
    if(datei==NULL)
    {
        puts("Error!\n");
    }
    else
    {
        if((*anz)==0)
        {
            puts("Wieviel Tests wollen sie eingben?(max 20)");
            scanf("%d",anz);
        }
        int i=0;
        while((fscanf(datei,"%d %d %d %s %d",&arr[i].datum.tag,&arr[i].datum.monat,&arr[i].datum.jahr,arr[i].gegenstand,&arr[i].punkte)>0) && ((i++)<20));


    }

    fclose(datei);
}

void speichern(Test *const arr, int *const anz)
{
    FILE *datei;
    char file[10];
    printf("Geben Sie einen Filenamen ein!\n");
    scanf("%s",file);
    datei=fopen(file,"w");
    if(datei==NULL)
        {
            puts("Error!\n");
        }
        else
        {
            if((*anz)==0)
            {
                puts("Wieviel Tests wollen sie eingben?(max 20)");
                scanf("%d",anz);
            }


            for(int i=0; i<(*anz); i++)
            {
                   fprintf(datei,"%d\n%d\n%d\n%s\n%d\n",arr[i].datum.tag,arr[i].datum.monat,arr[i].datum.jahr,arr[i].gegenstand,arr[i].punkte);
            }




        }
     fclose(datei);
}


void durchschnitt(Test *const arr, int *const anz)
{
    float durch=0;
    for(int i=0; i<(*anz); i++)
    {
        durch+=arr[i].punkte;
    }
    durch/=(*anz);
    printf("Der Durchschnitt aller Tests betraegt:%f",durch);
}



int main()
{
    Test arr[21];
    int anz=0;
    while(1)
    {
        switch(getch())
            {
                case '1':
                    eingeben(arr,&anz);
                    break;

                case '2':
                    ausgeben(arr,&anz);

                case '3':
                    einlesen(arr,&anz);

                case '4':
                    speichern(arr,&anz);

                case '5':
                    durchschnitt(arr,&anz);
                    break;
            }
    }

    return 0;
}
