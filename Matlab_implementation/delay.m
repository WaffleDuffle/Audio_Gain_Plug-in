close('all'), clear, clc;

[x, Fs] = audioread('Caramitru44kScurt.wav');

sound(x, Fs); 
pause(length(x)/Fs + 1);

g_dB = -10;
g = 10^(g_dB/20);

% gain implementation

for n = 1 : length(x)
    x(n) = x(n) * g;
end

sound(x, Fs); 
pause(length(x)/Fs + 1);


M_sec = 0.1 ;
M = M_sec * Fs;

% FIR delay
y = zeros(length(x), 1);
y(1:M) = x(1:M);
y(M+1:end) = x(M+1:end) + g*x(1:length(x)-M);
sound(y,Fs);
pause(length(y)/Fs + 1);
% FIR delay with circular buffer


M_max_sec = 10;
M_max = M_max_sec*Fs;
% delay line vector
dl = zeros(M_max, 1);  % column vector
index_IN = 1;

y = zeros(length(x), 1);
for n = 1 : length(x)
    index_OUT = index_IN - M;
    if index_OUT < 1
        index_OUT = M_max + index_OUT;
    end
    y(n) = x(n) + g*dl(index_OUT);
    dl(index_IN) = x(n);
    index_IN = index_IN + 1;
    if index_IN > M_max
        index_IN = 1;
    end
end

sound(y,Fs);
pause(length(y)/Fs + 1);

% IIR delay

g_dB = -3;
g = 10^(g_dB/20);

y = zeros(length(x), 1);
for n = 1 : length(x)
    index_OUT = index_IN - M;
    if index_OUT < 1
        index_OUT = M_max + index_OUT;
    end

    y(n) = x(n) + g*dl(index_OUT);
    dl(index_IN) = y(n);
    index_IN = index_IN + 1;

    if index_IN > M_max
        index_IN = 1;
    end
end

sound(y,Fs);
pause(length(y)/Fs + 1);