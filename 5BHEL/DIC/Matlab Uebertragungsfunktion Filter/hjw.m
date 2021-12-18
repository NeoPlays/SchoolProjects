%% Funktion hjw.m
function [hdb,hdeg]=hjw(bn, an, f)
%Calculate sinus function for angle vector phi
%A... Amplitude of sinus function
%phi... angle vector
%k... number of periods
%example y=skript_sinus(phi,A,k)
%length(x) berechnet Länge des Vektors
%hd Zählerpolynom 
%hn Nennerpolynom 
%h Übertragungsfunktion
%hdb Übertragungsfunktion in dB
%hdeg Übertragungsfunktion in Grad

%% Nennerpolynom 
%
% $$h_n = \sum_{i=0}^{n}{b_i * (jw)^i}$$
%
hn = 0;
for n = 1 :1: length(an)
    hn = hn* 1i*2*pi*f + an(n);
end

%% Zählerpolynom berechnen
%
% $$h_d = \sum_{i=0}^{n}{b_i * (jw)^i}$$
%
hd = 0;
for n = 1 :1: length(bn)
    hd = hd* 1i*2*pi*f + bn(n); 
end

%% Gesamtübertragungsfunktion berechnen
%
% $$h(j\omega)= \frac{h_d}{h_n}$$
%
% $$h(j\omega)_{dB}= 20*log{|h(j\omega)|}$$
%
% $$\varphi(h(j\omega))= \frac{180^\circ}{\pi}*\arg(h(j\omega))$$ 
%
h=hd/hn;
hdb = 20* log10(abs(h));
hdeg = 180/pi * angle(h);
end