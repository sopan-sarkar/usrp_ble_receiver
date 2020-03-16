function [demodData] = BLErecoveredData(filteredData)
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here
samplesPerSymbol = 4;
demodBits = gmskdemod(filteredData,samplesPerSymbol);
decodeData = int8(demodBits(1:end)>0);
receivedData = double(decodeData.');
demodData = binaryVectorToHex(receivedData,'LSBFirst');
demodData = flip(demodData);  