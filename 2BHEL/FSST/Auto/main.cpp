//Martin Platajs, 2BHEL, Auto
#include <stdlib.h>
#include <iostream>
#include <conio.h>

using namespace std;

class Auto
{
    int vmax;
    int v;
public:

    Auto()
    {
        vmax=0;
        v=0;
    }

    int get_vmax()
    {
        return vmax;
    }
    int get_v()
    {
        return v;
    }
    void set_vmax(int vmax1)
    {
        vmax=vmax1;
    }
    void set_v(int v1)
    {
        v=v1;
    }
    void beschleunigen(int ae)
    {
        v+=ae;
    }
    void bremsen(int ae)
    {
        v-=ae;
    }
};

int main()
{

    Auto bmw,audi;
    int vmax,ae;
    char ein,pedal;
    bool hilf=true;

    do{
        cout<<"Waehlen sie zwischen dem Audi und dem BMW (a/b) oder druecken sie s um zu beenden"<<endl;
        ein=getch();

        switch(ein)
        {

        case 'a':

            hilf=true;

            cout<<"Wie schnell beschleunigt/bremst ihr Auto?"<<endl;
            cin>>ae;

            cout<<"Was is das Maximale Tempo des Audis?"<<endl;
            cin>>vmax;
            audi.set_vmax(vmax);

            cout<<"Druecken sie a um zu beschleunigen, b um zu bremsen, und s um zu stoppen um ein anderes Auto auszusuchen."<<endl;

            while (hilf==true)
            {
                pedal=getch();
                system("CLS");
                if(audi.get_v()<vmax&&pedal=='a')
                {
                    audi.beschleunigen(ae);
                    cout<<audi.get_v()<<"km/h"<<endl;
                }
                else if(audi.get_v()>0&&pedal=='b')
                {
                    audi.bremsen(ae);
                    cout<<audi.get_v()<<"km/h"<<endl;
                }
                else if(pedal=='s')
                {
                    hilf=false;
                }
                else
                {
                    cout<<audi.get_v()<<"km/h"<<endl;
                }
            }
            break;


        case 'b':

            hilf=true;

            cout<<"Wie schnell beschleunigt/bremst ihr Auto?"<<endl;
            cin>>ae;

            cout<<"was ist das maximale Tempo des BMWs?"<<endl;
            cin>>vmax;
            bmw.set_vmax(vmax);

            cout<<"Druecken sie a um zu beschleunigen, b um zu bremsen und s um zu stoppen um ein anderes Auto auszusuchen."<<endl;

            while (hilf==true)
            {
                pedal=getch();
                system("CLS");
                if(bmw.get_v()<vmax&&pedal=='a')
                {
                    bmw.beschleunigen(ae);
                    cout<<bmw.get_v()<<"km/h"<<endl;

                }
                else if(bmw.get_v()>0&&pedal=='b')
                {
                    bmw.bremsen(ae);
                    cout<<bmw.get_v()<<"km/h"<<endl;


                }
                else if(pedal=='s')
                {
                    hilf=false;
                }
                else
                {
                    cout<<bmw.get_v()<<"km/h"<<endl;

                }
            }
            break;

        case 's':
            return 0;

        }


    }while(true);
    return 0;
}
