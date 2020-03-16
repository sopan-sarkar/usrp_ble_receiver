function [filteredData] = squelchedData(outChanFilt,SamplesPerFrame)
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
% Threshold for AGC
detectionThreshold = 0.01;
rxAmp = mean(abs(outChanFilt));
if rxAmp > detectionThreshold
    filteredData = outChanFilt;
else
    filteredData = complex(single(zeros(SamplesPerFrame, 1)));
end
end

