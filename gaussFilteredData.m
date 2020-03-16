function [rcvFilt] = gaussFilteredData(rcvdSignal,frameLength,SampleRate)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
%
%Initialize variables
rcvAGC = complex(zeros(frameLength,1));
rcvDCFree = complex(zeros(frameLength,1));
rcvFreqComp = complex(zeros(frameLength,1));
rcvFilt = complex(zeros(frameLength,1));
%

% GMSK demodulator design
samplesPerSymbol = 4;
span = 1;
BT = 0.5;

% Initialize System objects for receiver processing
agc = comm.AGC('MaxPowerGain',20,'DesiredOutputPower',2);

freqCompensator = comm.CoarseFrequencyCompensator('Modulation', 'OQPSK',...
                'SampleRate',SampleRate,...
                'SamplesPerSymbol',2*samplesPerSymbol,...
                'FrequencyResolution',100);

% Reference bleParam.h = gaussdesign(BT,span,samplesPerSymbol);
bleParam.h = gaussdesign(BT,span,samplesPerSymbol);

% AGC and channel selectivity
rcvAGC = agc(rcvdSignal); % Perform AGC
rcvDCFree = rcvAGC - mean(rcvAGC); % Remove the DC offset
rcvFreqComp = freqCompensator(rcvDCFree); % Estimate and compensate for the carrier frequency offset
rcvFilt = conv(rcvFreqComp,bleParam.h,'same'); % Perform gaussian matched filtering
release(freqCompensator);
end

