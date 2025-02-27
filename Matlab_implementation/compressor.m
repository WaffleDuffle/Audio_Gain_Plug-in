function output = compressor(input, fs, threshold_dB, ratio)  
    rmsAverage = 0.01; % 10 ms
    attackTime = 0.05; % 50 ms
    releaseTime = 0.2; % 200 ms 
    delayTime = 0.005; % 5 ms 
    delaySamples = round(delayTime * fs);  

    rmsLevel = 0;         
    smoothGain = 1;            % gain factor pentru smoothing
    buffer = zeros(1, delaySamples); % buffer circular
    bufferIndex = 1; 
    output = zeros(size(input)); 
    
    % constante de timp
    alphaAvg = exp(-1 / (fs * rmsAverage)); 
    alphaAt = exp(-1 / (fs * attackTime));   
    alphaRt = exp(-1 / (fs * releaseTime));  
    
    for i = 1:length(input)
        rmsLevel = alphaAvg * rmsLevel + (1 - alphaAvg) * input(i)^2;
        rmsLevel_dB = 10 * log10(rmsLevel + 0.0001);        

        if rmsLevel_dB > threshold_dB
            gain_dB = ratio * (threshold_dB - rmsLevel_dB);
        else
            gain_dB = 0; 
        end
        
        targetGain = 10^(gain_dB / 20); 
        
        if targetGain < smoothGain
            c = alphaAt; 
        else
            c = alphaRt; 
        end        
        % smoothing
        smoothGain = (1 - c) * smoothGain + c * targetGain;
      
        output(i) = smoothGain * buffer(bufferIndex);       
        buffer(bufferIndex) = input(i);   
        bufferIndex = mod(bufferIndex, delaySamples) + 1; 
    end
end