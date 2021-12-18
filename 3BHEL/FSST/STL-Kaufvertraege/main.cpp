/*
----------------------------------------------------------------------------------
-- Klasse:             3BHEL
-- Name:        < Platajs Martin >
--
-- Erstelldatum:     2019-03-27
-- Projekt-Name:    Kaufvertraege
-- Beschreibung: 	verkette Liste mit der STL-Libary für Kaufvertraege
----------------------------------------------------------------------------------
*/

#include "methoden.h"

using namespace std;
//Prototypen definition
void menue();
void auswahl();

//Menue bzw. Benutzeroberflaeche
void menue()
{
    cout<<"1. Einfuegen nach Nachanmen sortiert\n";
    cout<<"2. Ausgabe am Bildschirm aufsteigend sortiert(Start-Ende)\n";
    cout<<"3. Ausgabe am Bildschirm absteigend sortiert(Ende-Start)\n";
    cout<<"4. Einlesen von einer Datei\n";
    cout<<"5. Ausgabe in eine Datei\n";
    cout<<"6. Suchen nach Type\n";
    cout<<"7. Suchen nach Verbrauch\n";
    cout<<"8. Loeschen nach Nachname\n";
    cout<<"9. Loeschen nach Reifendimensionen\n";
    cout<<"0. Ende\n";
}



//Hauptprogramm
int main()
{
    Methoden liste;
    int par;
    do
    {
        menue();
        par=getch();
        switch(par)   //Auswahl des Menuepunkts
        {
            case '1':
                liste.erstellen();   //Eine Liste erstellen und einsortieren
                break;

            case '2':
                liste.ausgabe_forwaerts();     //Ausgabe aller Objekte (Anfang bis Ende)
                getch();
                break;

            case '3':
                liste.ausgabe_rueckwaerts();       //Ausgabe aller Objekte (Ende bis Anfang)
                getch();
                break;

            case '4':
                liste.einlesen();    //Objekte von einem File lesen
                getch();
                break;

            case '5':
                liste.speichern();     //Objekte auf ein File speichern
                getch();
                break;

            case '6':
                liste.suchen(par);   //alle Objekte mit bestimmtem Type suchen, Parameter-Uebergabe um zwischen Eigenschaften zu unterscheiden
                getch();
                break;

            case '7':
                liste.suchen(par);   //alle Objekte mit bestimmtem Verbrauch suchen, Parameter-Uebergabe um zwischen Eigenschaften zu unterscheiden
                getch();
                break;

            case '8':
                liste.loeschen(par);   //alle Objekte mit bestimmten Namen suchen und löschen, Parameter-Uebergabe um zwischen Eigenschaften zu unterscheiden
                getch();
                break;

            case '9':
                liste.loeschen(par);    //alle Objekte mit bestimmten Reifendimensionen suchen und löschen, Parameter-Uebergabe um zwischen Eigenschaften zu unterscheiden
                getch();
                break;
        }
        clrscr();
    }
    while(par!='0');

}
