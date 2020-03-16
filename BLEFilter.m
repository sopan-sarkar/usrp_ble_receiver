function [hd] = BLEFilter(SampleRate)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
% Filtering Design
%{
fd = fdesign.lowpass;
fd.Specification = 'N,Fp,Ap,Ast';
fd.FilterOrder = 100;
fd.Fpass = 600e3 * 2 / SampleRate; % normalized cutoff
fd.Apass = 1; % dB passband ripple
fd.Astop = 80; % dB stopband attenuation
ChannelFilter = design(fd, 'equiripple', 'SystemObject', true);
%ChannelFilterNumerator = ChannelFilter.Numerator;
%fvtool(ChannelFilter
%}
Fpass = 600e3 * 2 / SampleRate; % normalized cutoff
b = fir1(50,Fpass);     % 50th-order linear-phase FIR filter.
hd = dfilt.dffir(b);
end

