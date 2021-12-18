file = 'C:\Users\Plata\OneDrive - HTL Hollabrunn\5BHEL 2021\DIC\Matlab\Spektralanalsyse\Audio_Samples_Uebung#3\13_Piano\Piano1.wav';
[y,fs] = audioread(file); % load audio file
y1 = fliplr(y(:,1));
y1 = y1/max(y1);
t=0:1/fs:length(y1)/fs-1/fs; % create time vector
[yf,f] = calcSpectr(y1,fs,4410,@kaiser); % calculate spectrum

semilogy(f,yf); % log plot
title('Spektrum')
xlabel('Frequenz in Hz')
ylabel('Amplitude')
grid on
axis([20, 20000, 0, 0.5]);

