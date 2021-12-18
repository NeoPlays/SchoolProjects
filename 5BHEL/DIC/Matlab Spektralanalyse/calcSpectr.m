function[S_amp, f]=calcSpectr(s, fs, N, win)
% calculates Spectrum of Signal

% script output parameters:
% S_amp .. amplitude vector
% f ...... frequency vector

% script input parameters:
% s ...... signal vector
% fs ..... sampling frequency
% N ...... window size
% win .... window function


w = window(win,N); % create Window vector
s = s(10001:10000+N); % scale signal vector to window
s = s.*w'; % aply window

NFFT = N;
N = length(s);
if N>NFFT
    N=NFFT;
end

cutoff=ceil(NFFT/2);
S=fft(s,NFFT);
S_amp = abs(S);
S_amp = S_amp(1:cutoff);
S_amp = 2*S_amp/N;
f=(0:cutoff-1) .* fs/NFFT;
end