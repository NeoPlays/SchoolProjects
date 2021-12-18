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
#ifndef KAUFVERTRAG_H_INCLUDED
#define KAUFVERTRAG_H_INCLUDED

//Includes
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <list>
#include <conio.h>
#include "display.h"

using namespace std;


//Klasse für Kaufvertraege
class Kaufvertraege
{
	//Eigenschaften der Klasse
protected:
	string Name;
	string Marke;
	string Typ;
	float Verbrauch;
	string Reifendimensionen;
	int Eigengewicht;

	//Methoden
public:
	//Setzen eines ganzen Kaufvertrags
	void set_Kaufvertrag(string Name, string Marke, string Typ, float Verbrauch, string Reifendimensionen, int Eigengewicht)
	{
		this->Name = Name;
		this->Marke = Marke;
		this->Typ = Typ;
		this->Verbrauch = Verbrauch;
		this->Reifendimensionen = Reifendimensionen;
		this->Eigengewicht = Eigengewicht;
	}

	//Get-Methoden für jede einzelne Eigenschaft
	string get_Name() const {return Name;}
	string get_Marke() const {return Marke;}
	string get_Typ() const {return Typ;}
	float get_Verbrauch() const {return Verbrauch;}
	string get_Reifendimensionen() const {return Reifendimensionen;}
	int get_Eigengewicht() const {return Eigengewicht;}

	bool operator<(Kaufvertraege &k);

	//um Zugriff auf die Eigenschaften zu gewaerleisten
	friend void eingabe (Kaufvertraege &k);
	friend void ausgabe (const Kaufvertraege &iter);
};
bool Kaufvertraege :: operator<(Kaufvertraege &k){return Name < k.Name;}    //um den Befehl <Objekt>.sort() verwenden zu können

#endif // KAUFVERTRAG_H_INCLUDED
