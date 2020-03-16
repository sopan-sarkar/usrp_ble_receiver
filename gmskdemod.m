function z = gmskdemod(y,nSamp)
%GMSKDEMOD Gaussian minimum shift keying demodulation
%
%   Note: This is an internal undocumented function and its API and/or
%   functionality may change in subsequent releases.
%
%   Z = gmskdemod(Y,NSAMP) demodulates the complex envelope Y of a signal
%   using the Gaussian minimum shift keying method and produces soft
%   output Z. NSAMP denotes the number of samples per symbol and must be a
%   positive integer.

%   Copyright 2018-2019 The MathWorks, Inc.

%#codegen

sigLen = size(y,1);   % signal length in samples
nBits = sigLen/nSamp;  % signal length in bits

% Prepend the state
ini_state = complex(zeros(nSamp,1));
y = [ini_state;y];

% Perform gaussian matched filtering
bt = 0.5;
filtLen = 2*nSamp+1;
t = linspace(-1,1,filtLen);
alpha = sqrt(log(2)/2)/(bt);
h = (sqrt(pi)/alpha)*exp(-(t*pi/alpha).^2);
hNorm = h./sum(h);
yConv = conv2(y,hNorm(:),'same');

% Multiply the received GMSK signal by cos(pi*t/2T) and sin(pi*t/2T), where
% T is the symbol time. The phase offsets are different for the cos and
% sin multipliers to account for the circular shift in the I channel.
% These cos() and sin() multipliers are matched to the I and Q pulse
% shapes, respectively. Then perform receiver filtering. Assume that the
% received signal already is properly synchronized.

arg = pi * (0 : sigLen+nSamp-1)' / (2*nSamp);
multSig = complex(cos(arg+pi/2) .* real(yConv), sin(arg-pi/2) .* imag(yConv));

% Perform ideal integration
filtSig = (cumsum(multSig,1) - cumsum([zeros(2*nSamp,1); ...
    multSig(1:end-2*nSamp,:)],1)) / nSamp;

% Make I start from 2*nSamp and Q start from 3*nSamp. Account for the fact
% that the signal vector has been augmented in length.
Istart = 2 * nSamp;
Qstart = 3 * nSamp;
if (2*round(nBits/2) == nBits)
    Iend = sigLen;
    Qend = sigLen + nSamp;
else
    Iend = sigLen + nSamp;
    Qend = sigLen;
end

% Soft GMSK demodulation
zRealDec = real(filtSig(Istart:2*nSamp:Iend,:));
zImagDec = imag(filtSig(Qstart:2*nSamp:Qend,:));

% Upsample (i.e. repeat) the I and Q channels by 2, and shift the Q channel
% by 1 sample relative to the I channel. This operation allows for 2-bit
% decisions to be made over each I or Q symbol. Also, set the threshold
% for a suboptimal decision on the last bit.
zRealDec = kron(zRealDec,ones(2, 1));
zImagDec = kron(zImagDec,ones(2, 1));

if 2*round(nBits/2) == nBits
    threshold = 0.5 * sum(double(sign(real(filtSig(end-nSamp+1:end,:)))))/nSamp;
    zRealDec = [zRealDec(2:end,:); ...
        double(sign( real(filtSig(end,:)) - threshold ))];
else
    zRealDec = zRealDec(2:end,:);
    threshold = 0.5 * sum(double(sign(imag(filtSig(end-nSamp+1:end,:)))))/nSamp;
    zImagDec = [zImagDec; ...
    double(sign( imag(filtSig(end,:)) - threshold ))];
end

% Now multiply the real and imaginary parts together to find the signed
% data values
z = -zRealDec .* zImagDec;

end