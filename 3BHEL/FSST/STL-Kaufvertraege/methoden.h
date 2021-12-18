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
#ifndef METHODEN_H_INCLUDED
#define METHODEN_H_INCLUDED

#include "kaufvertrag.h"

//Eingabe aller Eigenschaften eines Objekts per Tastatur
void eingabe (Kaufvertraege &k)
{
	cout<<"Name: ";
	cin>>k.Name;
	cout<<"Marke: ";
	cin>>k.Marke;
	cout<<"Type: ";
	cin>>k.Typ;
	cout<<"Verbrauch: ";
	cin>>k.Verbrauch;
	cout<<"Reifendimensionen :";
	cin>>k.Reifendimensionen;
	cout<<"Eigengewicht: ";
	cin>>k.Eigengewicht;
	cout<<"\n";
}

//Ausgabe aller Eigenschaften eines Objekts
void ausgabe ( const Kaufvertraege &iter)
{
	cout<<iter.Name<<"\n";
	cout<<iter.Marke<<"\n";
	cout<<iter.Typ<<"\n";
	cout<<iter.Verbrauch<<"\n";
	cout<<iter.Reifendimensionen<<"\n";
	cout<<iter.Eigengewicht<<"\n\n";
}

//Klasse um Algorithmen der Liste zu programmieren
class Methoden
{
	list <Kaufvertraege> liste;     //Initialisierung der Liste
public:

	void erstellen()                //Eine Liste erstellen und einsortieren
	{
		Kaufvertraege Kaufvertrag;
		char wh;

		do
		{
			eingabe(Kaufvertrag);
			liste.push_back(Kaufvertrag);
			cout<<"Wiederhohlen ? J/N \n";
			cin>>wh;
			cout<<"\n";
		}
		while((wh == 'j') || (wh == 'J'));
			liste.sort();
	}

	void ausgabe_forwaerts()           //Ausgabe aller Objekte (Anfang bis Ende)
	{
		list <Kaufvertraege> :: iterator iter;

		for(iter = liste.begin(); iter != liste.end(); iter++)
		{
			ausgabe(*iter);
		}
	}

	void ausgabe_rueckwaerts()          //Ausgabe aller Objekte (Ende bis Anfang)
	{
		list <Kaufvertraege> :: iterator iter;

		liste.reverse();
		for(iter = liste.begin(); iter != liste.end(); iter++)
		{
			ausgabe(*iter);
		}
		liste.reverse();        //es wird jediglich die Liste reversiert, die Verhaltensweise des Iterators bliebt gleich
	}

	void einlesen()         //Objekte von einem File lesen
	{
		string file;
		//Buffer-Variablen
		string Name_buf;
		string Marke_buf;
		string Typ_buf;
		float Verbrauch_buf;
		string Reifendimensionen_buf;
		int Eigengewicht_buf;

		Kaufvertraege Kaufvertrag;

		cout<<"Geben sie einen Filenamen ein: ";
		cin>>file;

		ifstream datei(file);
		if(!datei)
		{
			cout<<"Error!";
		}
		else
		{
			while(!datei.eof())
			{
				datei>>Name_buf>>Marke_buf>>Typ_buf>>Verbrauch_buf>>Reifendimensionen_buf>>Eigengewicht_buf;
				Kaufvertrag.set_Kaufvertrag(Name_buf,Marke_buf,Typ_buf,Verbrauch_buf,Reifendimensionen_buf,Eigengewicht_buf);
				liste.push_back(Kaufvertrag);
			}
			liste.sort();
			datei.close();
		}
	}

	void speichern()            //Objekte auf ein File speichern
	{
		string file;
		list <Kaufvertraege> :: iterator iter;

		cout<<"Geben sie einen Filenamen ein: ";
		cin>>file;

		ofstream datei(file);
		if(!datei)
		{
			cerr<<"Error!";
		}
		else
		{
			for(iter = liste.begin(); iter != liste.end(); iter++)
			{
				datei<<iter->get_Name()<<"\n";
				datei<<iter->get_Marke()<<"\n";
				datei<<iter->get_Typ()<<"\n";
				datei<<iter->get_Verbrauch()<<"\n";
				datei<<iter->get_Reifendimensionen()<<"\n";
				datei<<iter->get_Eigengewicht()<<"\n\n";
			}
			datei.close();
		}
	}

	void suchen(int par)         //alle Objekte mit bestimmtem Type, Verbrauch suchen
	{
		list <Kaufvertraege> :: iterator iter;
		string string_buf;
		float float_buf;

		if(par == '6')          //Ermittlung der gewuenschten Eigenschaft
		{
			cout<<"Nach welchem Type wollen sie suchen? \n";
			cin>>string_buf;

			for(iter = liste.begin(); iter!= liste.end(); iter++)
			{
				if(iter->get_Typ() == string_buf)
				{
					ausgabe(*iter);
				}
			}
		}
		else if (par == '7')    //Ermittlung der gewuenschten Eigenschaft
		{
			cout<<"Nach welchem Verbrauch wollen sie suchen? \n";
			cin>>float_buf;

			for(iter = liste.begin(); iter!= liste.end(); iter++)
			{
				if(iter->get_Verbrauch() == float_buf)
				{
					ausgabe(*iter);
				}
			}
		}
	}

	void loeschen(int par)      //alle Objekte mit bestimmten Namen, Reifendimensionen suchen und löschen
	{
		string string_buf;

		if(par == '8')          //Ermittlung der gewuenschten Eigenschaft
		{
			cout<<"Nach welchem Namen wollen sie loeschen? \n";
			cin>>string_buf;
            liste.remove_if([&string_buf](const Kaufvertraege &Kaufvertrag){return Kaufvertrag.get_Name() == string_buf;});
            cout<<"Daten wurden geloescht!";
		}
		else if(par == '9')     //Ermittlung der gewuenschten Eigenschaft
		{
			cout<<"Nach welchen Reifendimensionen wollen sie loeschen? \n";
			cin>>string_buf;
            liste.remove_if([&string_buf](const Kaufvertraege &Kaufvertrag){return Kaufvertrag.get_Reifendimensionen() == string_buf;});
            cout<<"Daten wurden geloescht!";
		}
	}
};



#endif // METHODEN_H_INCLUDED
