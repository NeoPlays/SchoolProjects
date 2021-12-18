%% Funktion Transfer.m
function [hdb,hdeg,f] = Transfer(fch,ftyp,fmin,fmax,n,order,wg)
%hdb Vektor Amplitudengang
%hdeg Vektor Phasengang
%f Frequenzvektor
%fch Charakteristik (‘butter‘,‘cheby1‘, ‘cheby2‘ ‚‘bessel‘, ‘Cauer‘)
%ftyp ‘low‘,‘high‘, bandpass‘,‘stop‘
%fmin minmale Frequenz
%fmax maximale Frequenz
%n Anzahl der Frequenzpunkte
%order Filterordnung
%wg Kreisgrenzfrequenz

%% Mit switch wird ermittelt welche Funktionscharakteristik angewandt wird
    switch fch
        case 'butter'
            [bn,an] = butter(order, wg, ftyp, 's');
            type='Butterworth Pol-Nullstellen';
        case 'besself'

            [bn,an] = besself(order, wg, ftyp);
            type='Bessel Filter Pol-Nullstellen';
        otherwise
    end
    %% Pol-Nullstellen Diagramm erstellen
    
    %Wurzel der Filterkoeffizienten bn und an nehmen
    rbn = roots(bn);
    ran = roots(an);
    
    zplane(rbn, ran);
    title(type);
    grid on;
    
    %% dB und Grad Werte für alle Frequenzen in Vektor abspeichern
    u=0;
    f = fmin :(fmax-fmin)/n: fmax;

    for i = f       
        u = u + 1;
        [hdb(u), hdeg(u)] = hjw(bn, an, i);
    end
end