/// PERSONENSTRUKTUR
/// STEFAN GRUBMUELLER 2BHEL
/// 06.10.2017
#include <iostream>
#include <conio.h>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

typedef struct
{
    int monat;
    int tag;
    int jahr;
} Datum;

typedef struct
{
    string vorname;
    string nachname;
    Datum geb;
} Person;

void ausgabe (vector<Person>&menschen,const Person& pers);      //Prototypen
void eingabe (vector<Person>&menschen,Person& pers);
void alterpruf (vector<Person>&menschen, Person& pers);
void namepruf (vector<Person>menschen);
void lesenvf(vector <Person>&menschen,const Person&pers);
void schraf (vector <Person>&menschen,const Person&pers);
void menu ();




void menu ()            //menu
{
    cout<< "1.) Eingabe von Personen\n2.) Ausgabe aller Personen\n3.) Ausgabe aller volljaehrigen Personen\n4.) Sortieren nach Nachname\n5.) Lesen der Personen vom File\n6.) Schreiben der Personen auf ein File\n0.) Ende\n";
}


void eingabe (vector<Person>&menschen,Person& pers)     //Eingabe
{
    int anzahl;
    cout<< "Wieviele Personen wollen Sie eingeben? ";
    cin>>anzahl;            //Anzahl der Personen wird eingegeben
    menschen.resize(anzahl);         //anzahl wird festgelegt
    for (int i=0; i<anzahl; i++)     //Schleife fuer alle Personen
    {
        cout<<"Vorname: ";
        cin>>menschen.at(i).vorname;     //Daten werden eingegeben
        cin.sync();
        cout<<"Nachname: ";
        cin>>menschen.at(i).nachname;
        cin.sync();
        cout<<"Geburtsdatum in T/M/J: \n";
        cin>>menschen.at(i).geb.tag;
        cin>>menschen.at(i).geb.monat;
        cin>>menschen.at(i).geb.jahr;
        if(menschen.at(i).geb.tag>='32')
        {
            cout<< "error";
            getch();
        }
        clrscr();
    }
}



void ausgabe (vector<Person>&menschen,const Person& pers)   //Ausgabe
{
    for(int i=0; i<menschen.size(); i++) //Schleife fuer alle Personen
    {
        cout<<menschen.at(i).vorname<<' '<<menschen.at(i).nachname;     //Daten werden ausgegeben
        cout<<'\t'<< menschen.at(i).geb.tag<<'.'<<menschen.at(i).geb.monat<<'.'<<menschen.at(i).geb.jahr<<endl;
    }
    getch();        //Damit man es lesen kann
}

void alterpruf (vector<Person>&menschen, Person& pers)
{
    time_t zeit = time(0);      //aktuelle Zeit
    string zeit_string(ctime(&zeit));
    struct tm *zeitinfo = localtime (&zeit);
    for (int i=0; i<menschen.size(); i++)
    {
        if (menschen.at(i).geb.jahr<=(1900+zeitinfo->tm_year)-18)       //alter wird ueberprueft
        {
            cout<<menschen.at(i).vorname<<' '<< menschen.at(i).nachname;        //Daten werden ausgegeben
            cout<<'\t'<< menschen.at(i).geb.tag<<'.'<<menschen.at(i).geb.monat<<'.'<<menschen.at(i).geb.jahr<<endl;
        }
    }
    getch();
}


void namepruf(vector<Person>menschen)       //sortiert
{
    int j,help2;//hilfsvariablen
    string help;
    bool getauscht;
    j=menschen.size()-1;
    do      //bubble sort
    {
        j=j-1;
        getauscht = false;
        for(int i=0; i<=j; i++)
        {
            if(menschen.at(i).nachname>menschen.at(i+1).nachname)
            {
                help=menschen.at(i).nachname;
                menschen.at(i).nachname=menschen.at(i+1).nachname; //Gleichsetzten
                menschen.at(i+1).nachname=help;
                getauscht=true;
                //Auch Vorname:
                help=menschen.at(i).vorname;
                menschen.at(i).vorname=menschen.at(i+1).vorname; //Gleichsetzten
                menschen.at(i+1).vorname=help;
                //Auch Geburtsdatum: (Tag)
                help2=menschen.at(i).geb.tag;
                menschen.at(i).geb.tag=menschen.at(i+1).geb.tag; //Gleichsetzten
                menschen.at(i+1).geb.tag=help2;
                //Auch Geburtsdatum: (Monat)
                help2=menschen.at(i).geb.monat;
                menschen.at(i).geb.monat=menschen.at(i+1).geb.monat; //Gleichsetzten
                menschen.at(i+1).geb.monat=help2;
                //Auch Geburtsdatum: (Jahr)
                help2=menschen.at(i).geb.jahr;
                menschen.at(i).geb.jahr=menschen.at(i+1).geb.jahr; //Gleichsetzten
                menschen.at(i+1).geb.jahr=help2;
            }
        }
    }
    while((getauscht)&&(j>0));
    cout << "\n\n\n\n";
    cout<< "Nach Nachnamen sortiert."<<endl;        //Bestuetigung
    getch();
    clrscr();
}




void lesenvf(vector <Person>&menschen,const Person&pers)            //vom file lesen
{
    string fname;
    cout<<"Filename:";
    cin>>fname;     //Filename wird angegeben
    cin.sync();
    ifstream datei(fname);  //file wird inkludiert
    if(!datei)
    {
        cout << "ERROR";
    }
    else
    {
        while (!datei.eof())
        {
            menschen.resize(menschen.size()+1);     //Daten

            datei>>menschen.at(menschen.size()-1).vorname;
            datei>>menschen.at(menschen.size()-1).nachname;
            datei>>menschen.at(menschen.size()-1).geb.tag;
            datei>>menschen.at(menschen.size()-1).geb.monat;
            datei>>menschen.at(menschen.size()-1).geb.jahr;
        }
    }
    datei.close();
    clrscr();
    ausgabe(menschen,pers);     //oeffnet unterprogramm
    getch();
}

void schraf (vector <Person>&menschen,const Person&pers)        //Ausgabe aufs file
{
    string x;2
    cout<< "Wohin wollen sie die personen speichern?";
    cin>> x;        //Speicherort
    cin.sync();
    ofstream datei(x);
    if(!datei)
    {
        cout<<"Datei konnte nicht gefunden werden!";
    }
    else
    {
        for (int i=0; i<menschen.size(); i++)       //Daten werden ueberschrieben
        {
            datei<<menschen.at(i).vorname<<' '<<menschen.at(i).nachname<<'\t'<<menschen.at(i).geb.tag<<'.'<<menschen.at(i).geb.monat<<'.'<<menschen.at(i).geb.jahr;
        }
        datei.close;
    }
}

int main()      //Hauptprogramm
{
    vector <Person> menschen;       //Vector fuer die anzahl der Personen
    int ausw;
    Person pers;        //Personen werden definiert
    do      //Schleife fuer menu
    {
        clrscr();
        menu();     //Menu wird aufgerufen
        switch(getch())
        {
        case '1':       //Eingabe
            clrscr();
            eingabe(menschen,pers);
            break;

        case '2':       //Ausgabe
            clrscr();
            ausgabe(menschen,pers);
            break;

        case '3' :      //Alter wird ueberprueft
            clrscr();
            alterpruf(menschen,pers);
            break;

        case '4':       //namen werden sortiert
            clrscr();
            namepruf(menschen);
            break;

        case '5':       //wird vom file gelesen
            clrscr();
            lesenvf(menschen,pers);
            break;

        case '6':       //wird aufs file geschrieben
            clrscr();
            schraf(menschen,pers);
            break;

        case '0':       //Programm wird beendet
            clrscr();
            textcolor(BLACK);
            setcursortype(NOCURSOR);
            return 0;
            break;
        }

    }
    while (true);      //bedingung ist immer true
    return 0;
}
