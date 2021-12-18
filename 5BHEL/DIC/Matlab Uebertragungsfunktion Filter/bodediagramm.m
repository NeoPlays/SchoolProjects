%% Aufgabenstellung: Übertragungsfunktion eines analogen Filters
% Es sollen aus den definierten Variablen jeweils 4 Diagramme gezeichnet
% werden. Der Amplituden und Phasengang der Butterworth und der Besselfunktion
% Charaktaristik und die Pol-Nullstellen-Diagramme beider Charakteristiken.

%% Funktion Bodediagramm.m

%% Variablen definieren
lnr = 13;
order = 4+mod(lnr,3);
fg = 1000 * lnr;
wg = fg*2*pi;
fch = 'butter';
ftyp = 'high';
fmin = 1e-2*fg;
fmax = 10*fg;
n=500;

%% Pol-Nullstellen Diagramme zeichnen

figure('Position', [100, 100, 1024, 1024]);
subplot(2,2,1);
[hdb_butter,hdeg_butter,f_butter] = Transfer(fch,ftyp,fmin,fmax,n,order,wg);


fch = 'besself';
subplot(2,2,2);
[hdb_bessel,hdeg_bessel,f_bessel] = Transfer(fch,ftyp,fmin,fmax,n,order,wg);

%% Amplitudengänge über die Frequenz zeichnen
figure('Position', [100, 100, 1024, 1024]);
subplot(2,1,1);
semilogx(f_butter,hdb_butter, f_bessel,hdb_bessel);
axis([fmin fmax -220 10]);
title('Amplitudengang');
xlabel('Frequenz');
ylabel('Amplitude');
legend('butter','bessel');
grid on;

%% Phasengänge über die Frequenz zeichnen

subplot(2,1,2);
semilogx(f_butter,hdeg_butter, f_bessel,hdeg_bessel);
axis([fmin fmax -220 200]);
title('Phasengang');
xlabel('Frequenz');
ylabel('Phase');
legend('butter','bessel');
grid on;


