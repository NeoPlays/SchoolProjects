#include <iostream>
#include <cmath>

using namespace std;

class KZahlen
{
    float re,im;
public:

    KZahlen()
    {
        re=1;
        im=1;
    }

    float get_re()
    {
        return re;
    }

    float get_im()
    {
        return im;
    }

    void set_re(float real)
    {
        re=real;
    }

    void set_im(float imag)
    {
        im=imag;
    }

    KZahlen add(KZahlen &kz)
    {
        KZahlen erg;
        erg.set_re(re + kz.re);
        erg.set_im(im + kz.im);
        return erg;
    }

    KZahlen sub(KZahlen &kz)
    {
        KZahlen erg;
        erg.set_re(re - kz.re);
        erg.set_im(im - kz.im);
        return erg;
    }

    KZahlen mult(KZahlen &kz)
    {
        KZahlen erg;
        erg.set_re(re*kz.re - im*kz.im);
        erg.set_im(re*kz.im + im*kz.re);
        return erg;
    }

    KZahlen div(KZahlen &kz)
    {
        KZahlen erg;
        float nenner;

        nenner=pow(kz.re,2) + pow(kz.im,2);
        erg.set_re((re*kz.re - im*kz.im*(-1))/nenner);
        erg.set_im((re*kz.im - im*kz.re)/nenner);
        return erg;
    }

    KZahlen operator +(KZahlen &kz)
    {
        return add(kz);
    }

    KZahlen operator -(KZahlen &kz)
    {
        return sub(kz);
    }

    KZahlen operator *(KZahlen &kz)
    {
        return mult(kz);
    }

    KZahlen operator /(KZahlen &kz)
    {
        return div(kz);
    }
    friend ostream &operator <<(ostream &ausgabe,KZahlen &kz);
    friend istream &operator >>(istream &eingabe,KZahlen &kz);

};

ostream &operator <<(ostream &ausgabe,KZahlen &kz)
{
    return ausgabe<<kz.re<<"+"<<kz.im<<"j";
}

istream &operator >>(istream &eingabe,KZahlen &kz)
{
    float ein;
    cout<<"Realteil eingeben"<<endl;
    cin>>ein;
    kz.re=ein;
    cout<<"Imaginaerteil eingeben"<<endl;
    cin>>ein;
    kz.im=ein;
    return eingabe;
}


int main()
{
    KZahlen zahl1,zahl2,erg;
    char rz;

    cout<<"1.Zahl eingeben"<<endl;
    cin>>zahl1;

    cout<<"2.Zahl eingeben"<<endl;
    cin>>zahl2;

    cout<<"Rechenzeichen eingeben"<<endl;
    cin>>rz;

    switch(rz)
    {
    case '+':
        erg=zahl1+zahl2;
        cout<<erg<<endl;
        break;

    case '-':
        erg=zahl1-zahl2;
        cout<<erg<<endl;
        break;

    case '*':
        erg=zahl1*zahl2;
        cout<<erg<<endl;
        break;

    case '/':
        erg=zahl1/zahl2;
        cout<<erg<<endl;
        break;

    }


    return 0;

}
