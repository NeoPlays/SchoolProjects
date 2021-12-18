function calcSpectrV2(path, tstart, duration, win)
% calculates Spectrum of Signal

% script input parameters:
% path ...... audiofile path
% tstart .... start time in milliseconds
% duration .. duration in milliseconds
% win ....... window function

[y,fs] = audioread(path); % load audio file
s = fliplr(y(:,1));
s = s/max(s);
t=0:1/fs:length(s)/fs-1/fs; % create time vector

subplot(3,1,1) % plot original audio file
plot(t,s);
title('Audiofile')
xlabel('Zeit in Sekunden')
ylabel('Amplitude')
grid on
axis([0, length(s)/fs, -1.1*max(s), 1.1*max(s)])

N = duration/1000 * fs; % calculate samples
Ns = tstart/1000 * fs; % calculate start sample
NFFT = N;

w = window(win,N); % create window vector
s = s(Ns+1:Ns+N); % cut window
s = s/max(s); % normalize
s = s.*w; % apply window

t=tstart/1000:1/fs:tstart/1000+length(s)/fs-1/fs; % create time vector
subplot(3,1,2) % plot window
plot(t,s);
title('Fenster')
xlabel('Zeit in Sekunden')
ylabel('Amplitude')
grid on
axis([tstart/1000, (tstart+duration)/1000, -1, 1])

N = length(s);
if N>NFFT
N=NFFT;
end

cutoff=ceil(NFFT/2);
S=fft(s,NFFT);
S_mag = abs(S);
S_mag = S_mag(1:cutoff);
S_mag = 2*S_mag/N;

f=(0:cutoff-1) .* fs/NFFT;
fres = fs/N; % calculate frequency resolution
disp(strcat('frequency resolution: ', int2str(fres),' Hz'));

subplot(3,1,3) % plot spectrum
semilogy(f, S_mag);
title('Spektrum')
xlabel('Frequenz in Hz')
ylabel('Amplitude')
grid on
axis([20, 20000, 0, 0.5]);
end