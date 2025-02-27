close('all'), clear, clc

[x, Fs] = audioread('Caramitru44kScurt.wav');
t = 0:1/Fs:1;
sinus = linspace(0.1, 1, length(t)) .* sin(2*pi*400*t);
       
subplot(3,2,1);
hold on;
plot(sinus);
threshold_dB = -6;  
ratio = 2;    
y = compressor(sinus, Fs, threshold_dB, ratio);
plot(y);
title("raport 2:1, prag -6 dB");
hold off;

subplot(3,2,2);
hold on;
plot(x);
threshold_dB = -6;  
ratio = 2;    
y = compressor(x, Fs, threshold_dB, ratio);
plot(y);
title("raport 2:1, prag -6 dB");
hold off;
%sound(x, Fs);
%pause(length(x)/Fs + 1)
%sound(y, Fs);

subplot(3,2,3);
hold on;
plot(sinus);
threshold_dB = -15;  
ratio = 5;    
y = compressor(sinus, Fs, threshold_dB, ratio);
plot(y);
title("raport 5:1, prag -15 dB");
hold off;

subplot(3,2,4);
hold on;
plot(x);
threshold_dB = -15;  
ratio = 5;    
y = compressor(x, Fs, threshold_dB, ratio);
plot(y);
title("raport 5:1, prag -15 dB");
hold off;
grid on


%sound(x, Fs);
%pause(length(x)/Fs + 1)
%sound(y, Fs);
%pause(length(x)/Fs + 1)

subplot(3,2,5);
hold on;
plot(sinus);
threshold_dB = -18;  
ratio = 2;    
y = compressor(sinus, Fs, threshold_dB, ratio);
plot(y);
title("raport 2:1, prag -18 dB");
hold off;

%sound(sinus, Fs);
%pause(length(sinus)/Fs + 1)
%sound(y, Fs);


subplot(3,2,6);
hold on;
plot(x);
threshold_dB = -30;  
ratio = 2;    
y = compressor(x, Fs, threshold_dB, ratio);
plot(y);
title("raport 2:1, prag -18 dB");
hold off;
grid on
sound(x, Fs);
pause(length(x)/Fs + 1)
sound(y, Fs);

