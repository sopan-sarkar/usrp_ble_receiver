%{
p = [];
for i = 1:3
    t = datetime('now');
    t.Format = 'yyyy-MM-dd hh:mm:ss.SSS';
    y =  posixtime(t);
    p = [p; y];
    t.Format = 'yyyy-MM-dd hh:mm:ss.SSS';
    x = posixtime(t) + 25e-5;
    p = [p; x];
end

%a = [1 1 0 0 0 1 0 0 ; 0 0 0 0 1 0 1 0]
%pin = double(decodeData.');
%lin = binaryVectorToHex(pin,'LSBFirst');
%Min = binaryVectorToHex(pin,'MSBFirst');
%lin2 = flip(lin);
m = occuranceTimer.';
s = sum(m(2:end))
%}

p = gcp(); % Get the current parallel pool
f = parfeval(p,@rx_SDRu,1,3);
rcvdSignal = fetchOutputs(f);
disp(rcvdSignal);