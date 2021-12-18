//Auswahl zwischen den einzelnen Menuepunkten, Martin Platajs, 3BHEL
#include "header.h"
#include <conio.h>

void auswahl()
{
    Kaufvertrag *start = NULL;
    Kaufvertrag *ende = NULL;
    int parameter;

    do
    {
        menue();
        parameter=getch();
        switch(parameter)   //Auswahl des Menuepunkts
        {
            case '1':
                erstellen(&start, &ende);   //Aufruf des UPs erstellen
                break;

            case '2':
                ausgabeaufsteig(start);     //Aufruf des UPs ausgabeaufsteig
                getch();
                break;

            case '3':
                ausgabeabsteig(ende);       //Aufruf des UPs ausgabeabsteig
                getch();
                break;

            case '4':
                einlesen(&start, &ende);    //Aufruf des UPs einlesen
                getch();
                break;

            case '5':
                speichern(start, ende);     //Aufruf des UPs speichern
                getch();
                break;

            case '6':
                suchen(&start, &ende, parameter);   //Aufruf des UPs suchen
                getch();
                break;

            case '7':
                suchen(&start, &ende, parameter);   //Aufruf des UPs suchen
                getch();
                break;

            case '8':
                suchen(&start, &ende, parameter);   //Aufruf des UPs suchen
                getch();
                break;

            case '9':
                suchen(&start,&ende, parameter);    //Aufruf des UPs suchen
                getch();
                break;
        }
    }
    while(parameter!='0');



}
